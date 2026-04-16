#ifndef ARRAY_UNIVERSAL_PTWS_H
#define ARRAY_UNIVERSAL_PTWS_H

#include "array.hpp"
#include "simd.hpp"

namespace ArrayLibrary
{
    template <auto f, typename ResultType, typename... InputTypes>
    concept ScalarFunction = requires(const InputTypes... inputs) {
        { f(inputs...) } -> std::same_as<ResultType>;
    };

    template <auto f, typename ResultType, typename... InputTypes>
    concept SimdFunction = requires(const Simd::Vector<InputTypes>... inputs) {
        { f(inputs...) } -> std::same_as<Simd::Vector<ResultType>>;
    };

    template <typename Operation>
    concept HasSimd = requires {
        Operation::fSimd;
        requires(!(requires { requires Operation::ignoreSimd; }));
    };

    template <typename Operation>
        requires requires { std::function{Operation::f}; }
    using OperationResultType = decltype(std::function{Operation::f})::result_type;

    /// A class Operation satisfies IsParametrizedOperation with given input types if it has a static callable f that takes objects of the input types as input and produces a DataType output. Furthermore, if the class Operation satisfies HasSimd<Operation>, then fSimd objects of the type Simd::Vector<InputType> for each input type of f as input and produces Simd::Vector<T> where T is the result type of f.
    template <typename Operation, typename... InputTypes>
    concept IsNonParametrizedOperation = requires {
        requires requires(const InputTypes... inputs) {
            { Operation::f(inputs...) } -> DataType;
        };

        requires(!HasSimd<Operation>) || requires(const Simd::Vector<InputTypes>... inputs) {
            { Operation::fSimd(inputs...) } -> std::same_as<Simd::Vector<OperationResultType<Operation>>>;
        };

        Operation();
    };

    /// A class Operation satisfies IsParametrizedOperation with given input types if it has a non-static field param and a static callable f that takes param and objects of the input types as input and produces a DataType output. Furthermore, if the class Operation satisfies HasSimd<Operation>, then it must also have a private non-static field simdParam such that fSimd takes simdParam and objects of the type Simd::Vector<InputType> for each input type of f as input and produces Simd::Vector<T> where T is the result type of f.
    template <typename Operation, typename... InputTypes>
    concept IsParametrizedOperation = requires(const Operation &info) {
        requires requires(const InputTypes... inputs) {
            { Operation::f(info.param, inputs...) } -> DataType;
        };

        requires(!HasSimd<Operation>) || requires(const Simd::Vector<InputTypes>... inputs) {
            { Operation::fSimd(info.simdParam, inputs...) } -> std::same_as<Simd::Vector<OperationResultType<Operation>>>;
        };
    };

    template <typename Operation, typename... InputTypes>
    concept IsOperation = IsNonParametrizedOperation<Operation, InputTypes...> || IsParametrizedOperation<Operation, InputTypes...>;

    template <std::same_as<Coordinates>... CoordsPack>
    inline void checkMovingHint(const long lastOuterAxis, const long flatBoostAxisLength, const std::pair<const Coordinates &, const Coordinates &> &destInfo, const std::pair<const CoordsPack &, const CoordsPack &> &...remainderPack)
    {
    }

    /// @brief Checks whether the moving hints are accurate
    /// @tparam FirstHint
    /// @tparam ...RemainingHints
    /// @tparam ...CoordsPack
    /// @param lastOuterAxis The last axis that is not flattened during computation
    /// @param flatBoostAxisLength The combined length of the flattened axes
    /// @param destInfo
    /// @param firstSourceInfo
    /// @param ...remainderPack
    template <bool FirstHint, bool... RemainingHints, std::same_as<Coordinates>... CoordsPack>
        requires(sizeof...(RemainingHints) <= sizeof...(CoordsPack))
    inline void checkMovingHint(const long lastOuterAxis, const long flatBoostAxisLength, const std::pair<const Coordinates &, const Coordinates &> &destInfo, const std::pair<const Coordinates &, const Coordinates &> &firstSourceInfo, const std::pair<const CoordsPack &, const CoordsPack &> &...remainderPack)
    {
        const Coordinates &destShape = destInfo.first;
        const Coordinates &destStrides = destInfo.second;
        const Coordinates &sourceShape = firstSourceInfo.first;
        const Coordinates &sourceStrides = firstSourceInfo.second;

        const bool match = (sourceStrides[lastOuterAxis + 1] == destStrides[lastOuterAxis + 1] && sourceShape[lastOuterAxis + 1] == destShape[lastOuterAxis + 1]);

        if (match == FirstHint)
            checkMovingHint<RemainingHints...>(lastOuterAxis, flatBoostAxisLength, destInfo, remainderPack...);
        else
            throw std::invalid_argument("Source arrays do not have correct shape for moving hint.");
    }

    inline bool checkMatch(const Coordinates &sourceShape, const Coordinates &sourceStrides, const long lastOuterAxis, const long lastNonTrivialAxis, const long lengthTarget, const long matchFlatLength)
    {
        bool skip = sourceShape[lastNonTrivialAxis] == 1;
        return (skip && sourceShape[lastOuterAxis] == 1) || (!skip && sourceStrides[lastOuterAxis] == matchFlatLength && sourceShape[lastOuterAxis] == lengthTarget);
    }

    /// @brief Computes pointwise N-ary operations on arrays.
    /// @tparam ResultType
    /// @tparam ...InputTypes
    template <DataType ResultType, DataType... InputTypes>
    class UniversalPointwise
    {
        UniversalPointwise() = delete;

        static constexpr size_t N = sizeof...(InputTypes);
        static constexpr size_t LARGEST_TYPE_SIZE = std::max({sizeof(ResultType), sizeof(InputTypes)...});
        static constexpr size_t INCREMENT = SIMD_BYTES / LARGEST_TYPE_SIZE;
        static constexpr bool SIMD_SUPPORTED = (... && Simd::supported<InputTypes>) && Simd::supported<ResultType>;

        template <DataType T>
        struct SourceInfo
        {
            const T *pData;
            const Coordinates &shape;
            const Coordinates &strides;

            SourceInfo(const SourceInfo<T> &other) : pData(other.pData), shape(other.shape), strides(other.strides) {}

            SourceInfo(const Array<T> &array) : pData(array.readDataPointer()), shape(array.refShape()), strides(array.refStrides()) {}

            inline const T value() const { return *pData; }

            template <bool Moving>
            inline void innerAdvance()
            {
                if (Moving)
                    pData++;
            }

            inline void outerAdvance(long i)
            {
                pData += strides[i];
            }

            inline void reset(long i)
            {
                pData -= strides[i] * (shape[i] - 1);
            }
        };

        template <DataType T>
        struct SimdSourceInfo
        {
            const T *pData;
            Simd::Vector<T> current;
            const Coordinates &shape;
            const Coordinates &strides;

            SimdSourceInfo(const SimdSourceInfo<T> &other) : pData(other.pData), shape(other.shape), strides(other.strides), current(other.current) {}

            SimdSourceInfo(const Array<T> &array) : pData(array.readDataPointer()), shape(array.refShape()), strides(array.refStrides()), current(Simd::broadcast_set<T>(*pData)) {}

            inline const Simd::Vector<T> &value() const { return current; }

            template <bool Moving>
            inline void innerAdvance()
            {
                if constexpr (Moving)
                {
                    current = Simd::spreadLoad<T, LARGEST_TYPE_SIZE>(pData);
                    pData += INCREMENT;
                }
            }

            template <bool Moving>
            inline void outerAdvance(long i)
            {
                pData += strides[i];
                if constexpr (Moving)
                {
                    current = Simd::spreadLoad<T, LARGEST_TYPE_SIZE>(pData);
                }
                else
                {
                    current = Simd::broadcast_set<T>(*pData);
                }
            }

            inline void reset(long i)
            {
                pData -= strides[i] * (shape[i] - 1);
            }
        };

        template <typename Operation, bool... Moving>
            requires(IsOperation<Operation, InputTypes...>)
        inline static void innerLoop(const Operation &opInfo, long length, ResultType *pDest, SourceInfo<InputTypes>... sourceInfos)
        {
            for (long j = 0; j < length; j++)
            {
                if constexpr (IsNonParametrizedOperation<Operation, InputTypes...>)
                    *pDest++ = Operation::f(sourceInfos.value()...);
                else
                    *pDest++ = Operation::f(opInfo.param, sourceInfos.value()...);
                ((sourceInfos.template innerAdvance<Moving>()), ...);
            }
        }

        template <typename Operation, bool... Moving>
            requires(sizeof...(Moving) == N && IsOperation<Operation, InputTypes...>)
        inline static void outerLoop(const Operation &opInfo, const long lastOuterAxis, const long flatBoostAxisLength, Array<ResultType> &dest, SourceInfo<InputTypes> &&...sourceInfos)
        {
            ResultType *pDestData = dest.getDataPointer();
            const Coordinates &destShape = dest.refShape();
            const Coordinates &destStrides = dest.refStrides();

            Coordinates c(lastOuterAxis + 1, 0);
            bool end = false;

            while (!end)
            {
                innerLoop<Operation, Moving...>(opInfo, flatBoostAxisLength, pDestData, sourceInfos...);

                end = true;

                for (long i = lastOuterAxis; i >= 0; i--)
                {
                    c[i]++;

                    if (c[i] != destShape[i])
                    {
                        ((sourceInfos.outerAdvance(i)), ...);
                        pDestData += destStrides[i];
                        end = false;
                        break;
                    }
                    else
                    {
                        (sourceInfos.reset(i), ...);
                        pDestData -= destStrides[i] * (destShape[i] - 1);
                        c[i] = 0;
                    }
                }
            }
        }

        template <typename Operation, bool... Moving>
            requires(IsOperation<Operation, InputTypes...>)
        inline static void simdInnerLoop(const Operation &opInfo, long length, ResultType *pDest, SimdSourceInfo<InputTypes>... sourceInfos)
        {
            size_t j = 0;
            Simd::Vector<ResultType> result;

            for (j = 0; j + INCREMENT <= length; j += INCREMENT)
            {
                ((sourceInfos.template innerAdvance<Moving>()), ...);

                if constexpr (IsNonParametrizedOperation<Operation, InputTypes...>)
                    result = Operation::fSimd(sourceInfos.value()...);
                else
                    result = Operation::fSimd(opInfo.simdParam, sourceInfos.value()...);

                Simd::spreadStore<ResultType, LARGEST_TYPE_SIZE>(pDest, result);
                pDest += INCREMENT;
            }

            if (j < length)
            {
                ((sourceInfos.template innerAdvance<Moving>()), ...);

                if constexpr (IsNonParametrizedOperation<Operation, InputTypes...>)
                    result = Operation::fSimd(sourceInfos.value()...);
                else
                    result = Operation::fSimd(opInfo.simdParam, sourceInfos.value()...);

                Simd::spreadStore<ResultType, LARGEST_TYPE_SIZE>(pDest, result, length - j);
            }
        }

        template <typename Operation, bool... Moving>
            requires(sizeof...(Moving) == N && IsOperation<Operation, InputTypes...> && HasSimd<Operation>)
        inline static void simdOuterLoop(const Operation &opInfo, const long lastOuterAxis, const long flatBoostAxisLength, Array<ResultType> &dest, SimdSourceInfo<InputTypes> &&...sourceInfos)
        {
            ResultType *pDestData = dest.getDataPointer();
            const Coordinates &destShape = dest.refShape();
            const Coordinates &destStrides = dest.refStrides();

            Coordinates c(destShape.size() - 1, 0);
            bool end = false;

            while (!end)
            {
                simdInnerLoop<Operation, Moving...>(opInfo, flatBoostAxisLength, pDestData, sourceInfos...);

                end = true;

                for (long i = lastOuterAxis; i >= 0; i--)
                {
                    c[i]++;

                    if (c[i] != destShape[i])
                    {
                        (sourceInfos.template outerAdvance<Moving>(i), ...);
                        pDestData = destShape[i] == 1 ? pDestData : pDestData + destStrides[i];
                        end = false;
                        break;
                    }
                    else
                    {
                        (sourceInfos.reset(i), ...);
                        pDestData = destShape[i] == 1 ? pDestData : pDestData - destStrides[i] * (destShape[i] - 1);
                        c[i] = 0;
                    }
                }
            }
        }

        template <typename Operation, bool... Moving>
            requires(sizeof...(Moving) == N && IsOperation<Operation, InputTypes...>)
        static void execute(const Operation &opInfo, bool simd, const long lastOuterAxis, const long flatBoostAxisLength, Array<ResultType> &dest, const Array<InputTypes> &...sources)
        {
            if constexpr (HasSimd<Operation>)
            {
                if (simd)
                {
                    simdOuterLoop<Operation, Moving...>(opInfo, lastOuterAxis, flatBoostAxisLength, dest, SimdSourceInfo<InputTypes>(sources)...);
                    return;
                }
            }
            outerLoop<Operation, Moving...>(opInfo, lastOuterAxis, flatBoostAxisLength, dest, SourceInfo<InputTypes>(sources)...);
        }

        template <typename Operation, bool... Moving>
            requires(sizeof...(Moving) < N && IsOperation<Operation, InputTypes...>)
        static void execute(const Operation &opInfo, bool simd, const long lastOuterAxis, const long flatBoostAxisLength, Array<ResultType> &dest, const Array<InputTypes> &...sources)
        {
            auto &source = refPackGet<sizeof...(Moving)>(sources...);

            if (lastOuterAxis != source.getDim() - 1 && lastOuterAxis >= -1 && source.refStrides()[lastOuterAxis + 1] == dest.refStrides()[lastOuterAxis + 1] && source.refShape()[lastOuterAxis + 1] == dest.refShape()[lastOuterAxis + 1])
                execute<Operation, Moving..., true>(opInfo, simd, lastOuterAxis, flatBoostAxisLength, dest, sources...);
            else
                execute<Operation, Moving..., false>(opInfo, simd, lastOuterAxis, flatBoostAxisLength, dest, sources...);
        }

        template <typename Operation, bool... MovingHints>
            requires(IsOperation<Operation, InputTypes...>)
        static Array<ResultType> &dispatch(const Operation &opInfo, Array<ResultType> &dest, const Array<InputTypes> &...sources)
        {
            assertm((... && isSubshape(sources.refShape(), dest.refShape())), "Not all source arrays have a shape that is a subshape of the shape of the destination array.");

            if (dest.getDim() == 0)
            {
                if constexpr (IsNonParametrizedOperation<Operation, InputTypes...>)
                    dest.mData[dest.mOffset] = Operation::f((sources.eval())...);
                else
                    dest.mData[dest.mOffset] = Operation::f(opInfo.param, (sources.eval())...);
                return dest;
            }

            const Coordinates &destShape = dest.refShape();
            const Coordinates &destStrides = dest.refStrides();

            constexpr long flatLengthThreshold = 8;

            long lastNonTrivialAxis = dest.getDim() - 1;
            while (destShape[lastNonTrivialAxis] == 1 && lastNonTrivialAxis > 0)
                lastNonTrivialAxis--;

            long lastOuterAxis = lastNonTrivialAxis;
            long matchFlatLength = 1;

            while (lastOuterAxis >= 0)
            {
                if (destStrides[lastOuterAxis] != matchFlatLength)
                    break;

                if (!(... && checkMatch(sources.refShape(), sources.refStrides(), lastOuterAxis, lastNonTrivialAxis, destShape[lastOuterAxis], matchFlatLength)))
                    break;

                matchFlatLength *= destShape[lastOuterAxis];
                lastOuterAxis--;
            }

            if (lastOuterAxis < lastNonTrivialAxis)
                checkMovingHint<MovingHints...>(lastOuterAxis, matchFlatLength, std::pair<const Coordinates &, const Coordinates &>(dest.refShape(), dest.refStrides()), std::pair<const Coordinates &, const Coordinates &>(sources.refShape(), sources.refStrides())...);

            if (HasSimd<Operation> && SIMD_SUPPORTED && matchFlatLength >= 8)
                execute<Operation, MovingHints...>(opInfo, true, lastOuterAxis, matchFlatLength, dest, sources...);
            else
                execute<Operation, MovingHints...>(opInfo, false, lastOuterAxis, matchFlatLength, dest, sources...);

            return dest;
        }

    public:
        template <typename Operation, bool... MovingHints>
            requires(IsNonParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= N)
        static Array<ResultType> &computeInPlace(Array<ResultType> &dest, const Array<InputTypes> &...sources)
        {
            return dispatch<Operation, MovingHints...>(Operation(), dest, (sources.adjustDimension(dest.getDim()))...);
        }

        template <typename Operation, bool... MovingHints>
            requires(IsParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= N)
        static Array<ResultType> &computeInPlace(const Operation &operation, Array<ResultType> &dest, const Array<InputTypes> &...sources)
        {
            return dispatch<Operation, MovingHints...>(operation, dest, (sources.adjustDimension(dest.getDim()))...);
        }

        template <typename Operation, bool... MovingHints>
            requires(IsNonParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= N)
        static Array<ResultType> compute(const Array<InputTypes> &...sources)
        {
            auto resultShape = findOuterShape(sources.refShape()...);
            auto result = Array<ResultType>(Data<ResultType>(Array<ResultType>::calculateFlatLength(resultShape)), resultShape);

            return dispatch<Operation, MovingHints...>(Operation(), result, (sources.leftExpandDim(result.getDim() - sources.getDim()))...);
        }

        template <typename Operation, bool... MovingHints>
            requires(IsParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= N)
        static Array<ResultType> compute(const Operation &operation, const Array<InputTypes> &...sources)
        {
            auto resultShape = findOuterShape(sources.refShape()...);
            auto result = Array<ResultType>(Data<ResultType>(Array<ResultType>::calculateFlatLength(resultShape)), resultShape);

            return dispatch<Operation, MovingHints...>(operation, result, (sources.leftExpandDim(result.getDim() - sources.getDim()))...);
        }
    };

    // The purpose of these functions is that users have to enter as few redundant template types as possible.

    /// @brief Computes an N-ary pointwise operation and stores the result in a pre-existing destination array.
    /// @tparam Operation An operation class for a computation that does NOT have a parameter. The class should have a (public) static callable f that takes instances of the input type and produces a DataType output. See common_operations.hpp for examples. The class may also have a static callable Operation::fSimd that is a Simd::Vector version of f.
    /// @tparam ...MovingHints An optional hint for each input to reduce compilation time and binary size. Set to true for those shapes where the last dimension does not need to be broadcasted. For example, if the inputs are x and y (in this order), x has shape {1, 10}, and y has shape {10, 1}, the moving hint should be true, false.
    /// @tparam ...InputTypes The DataTypes of the pointwise computation inputs.
    /// @param dest The destination array whose underlying DataType has to match the output type of Operation::f.
    /// @param ...sources The input arrays whose underlying DataTypes has to match the input types of the computation.
    /// @return A reference to the passed destination array.
    template <typename Operation, bool... MovingHints, DataType... InputTypes>
        requires(IsNonParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= sizeof...(InputTypes))
    Array<OperationResultType<Operation>> &computeInPlace(Array<OperationResultType<Operation>> &dest, const Array<InputTypes> &...sources)
    {
        if (!(... && isExtensionSubshape(sources.refShape(), dest.refShape())))
            throw std::invalid_argument("Not all source arrays have a shape that is a subshape of the shape of the destination array.");
        return UniversalPointwise<OperationResultType<Operation>, InputTypes...>::template computeInPlace<Operation, MovingHints...>(dest, sources...);
    }

    /// @brief Computes an N-ary pointwise operation and stores the result in a pre-existing destination array.
    /// @tparam Operation An operation class for a computation that DOES have a parameter. The class should have a (public) static callable f that takes instances of the input type and produces a DataType output. See common_operations.hpp for examples. The class may also have a static callable Operation::fSimd that is a Simd::Vector version of f.
    /// @tparam ...MovingHints An optional hint for each input to reduce compilation time and binary size. Set to true for those shapes where the last dimension does not need to be broadcasted. For example, if the inputs are x and y (in this order), x has shape {1, 10}, and y has shape {10, 1}, the moving hint should be true, false.
    /// @tparam ...InputTypes The DataTypes of the pointwise computation inputs.
    /// @param operation An instance of Operation which serves to insert the parameter that is to be applied in each pointwise operation.
    /// @param dest The destination array whose underlying DataType has to match the output type of Operation::f.
    /// @param ...sources The input arrays whose underlying DataTypes has to match the input types of the computation.
    /// @return A reference to the passed destination array.
    template <typename Operation, bool... MovingHints, DataType... InputTypes>
        requires(IsParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= sizeof...(InputTypes))
    Array<OperationResultType<Operation>> &computeInPlace(const Operation &operation, Array<OperationResultType<Operation>> &dest, const Array<InputTypes> &...sources)
    {
        if (!(... && isExtensionSubshape(sources.refShape(), dest.refShape())))
            throw std::invalid_argument("Not all source arrays have a shape that is a subshape of the shape of the destination array.");
        return UniversalPointwise<OperationResultType<Operation>, InputTypes...>::template computeInPlace<Operation, MovingHints...>(operation, dest, sources...);
    }

    /// @brief Computes an N-ary pointwise operation and stores the result in a newly created array.
    /// @tparam Operation An operation class for a computation that does NOT have a parameter. The class should have a (public) static callable f that takes instances of the input type and produces a DataType output. See common_operations.hpp for examples. The class may also have a static callable Operation::fSimd that is a Simd::Vector version of f.
    /// @tparam ...MovingHints An optional hint for each input to reduce compilation time and binary size. Set to true for those shapes where the last dimension does not need to be broadcasted. For example, if the inputs are x and y (in this order), x has shape {1, 10}, and y has shape {10, 1}, the moving hint should be true, false.
    /// @tparam ...InputTypes The DataTypes of the pointwise computation inputs.
    /// @param ...sources The input arrays whose underlying DataTypes has to match the input types of the computation.
    /// @return The newly created array.
    template <typename Operation, bool... MovingHints, DataType... InputTypes>
        requires(IsNonParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= sizeof...(InputTypes))
    Array<OperationResultType<Operation>> compute(const Array<InputTypes> &...sources)
    {
        if (!isExtensionBroadcastable(sources.refShape()...))
            throw std::invalid_argument("The shapes of the input arrays cannot be broadcasted to match.");
        return UniversalPointwise<OperationResultType<Operation>, InputTypes...>::template compute<Operation, MovingHints...>(sources...);
    }

    /// @brief Computes an N-ary pointwise operation and stores the result in a newly created array.
    /// @tparam Operation An operation class for a computation that does NOT have a parameter. The class should have a (public) static callable f that takes instances of the input type and produces a DataType output. See common_operations.hpp for examples. The class may also have a static callable Operation::fSimd that is a Simd::Vector version of f.
    /// @tparam ...MovingHints An optional hint for each input to reduce compilation time and binary size. Set to true for those shapes where the last dimension does not need to be broadcasted. For example, if the inputs are x and y (in this order), x has shape {1, 10}, and y has shape {10, 1}, the moving hint should be true, false.
    /// @tparam ...InputTypes The DataTypes of the pointwise computation inputs.
    /// @param operation An instance of Operation which serves to insert the parameter that is to be applied in each pointwise operation.
    /// @param ...sources The input arrays whose underlying DataTypes has to match the input types of the computation.
    /// @return The newly created array.
    template <typename Operation, bool... MovingHints, DataType... InputTypes>
        requires(IsParametrizedOperation<Operation, InputTypes...> && sizeof...(MovingHints) <= sizeof...(InputTypes))
    Array<OperationResultType<Operation>> compute(const Operation &operation, const Array<InputTypes> &...sources)
    {
        if (!isExtensionBroadcastable(sources.refShape()...))
            throw std::invalid_argument("The shapes of the input arrays cannot be broadcasted to match.");
        return UniversalPointwise<OperationResultType<Operation>, InputTypes...>::template compute<Operation, MovingHints...>(operation, sources...);
    }
}
#endif
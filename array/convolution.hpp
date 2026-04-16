#ifndef CONVOLUTION_H
#define CONVULTION_H

#include "array.hpp"

namespace ArrayLibrary::Convolution
{
    enum class Padding
    {
        SHRINK,
        POST_ZERO,
        PRE_ZERO
    };

    struct Settings
    {
        bool keepInnerProductAxis;
        long innerProductAxis; //(channel axis)
        StackBuffer<bool, MAX_DIM> &convAxes;
        Padding padding;
    };

    Coordinates
    convShapePostExpansion(const Coordinates &arrayShape, const Coordinates &kernelShape, const Settings &settings)
    {
        long dim = arrayShape.size();
        if (kernelShape.size() != dim)
            throw std::invalid_argument("Kernel shape must have the same number of dimensions as the array shape.");

        if (settings.convAxes[settings.innerProductAxis])
            throw std::invalid_argument("Inner product axis cannot be a convolution axis.");

        Coordinates newShape(settings.keepInnerProductAxis ? dim : dim - 1);
        long j = 0;
        for (long i = 0; i < dim; i++)
        {
            if (i == settings.innerProductAxis)
            {
                if (arrayShape[i] != kernelShape[i])
                    throw std::invalid_argument("Array and kernel shapes must match in the inner product axis.");
                if (settings.keepInnerProductAxis)
                    newShape[j++] = 1; // Keep the inner product axis as a singleton dimension
                else
                    continue; // Skip the inner product axis
            }
            else if (settings.convAxes[i])
            {
                if (arrayShape[i] < kernelShape[i])
                    throw std::invalid_argument("Array shape must be at least as large as kernel shape in convolution axes.");
                if (settings.padding == Padding::SHRINK)
                {
                    newShape[j++] = arrayShape[i] - kernelShape[i] + 1; // Shrink the dimension
                }
                else
                {
                    newShape[j++] = arrayShape[i]; // Keep the dimension size
                }
            }
            else if (arrayShape[i] == 1)
            {
                newShape[j++] = kernelShape[i]; // Dimension of filters
            }
            else if (kernelShape[i] == 1)
            {
                newShape[j++] = arrayShape[i]; // Dimension of input
            }
            else
                throw std::invalid_argument("Non-convolution axes must be broadcastable between array and kernel shapes.");
        }

        return newShape;
    }

    template <DataType T>
    void simpleConvolveInner(const Array<T> &dest, const Array<T> &array, const Array<T> &kernel, const Coordinates& movingShape, const Coordinates &kernelPos)
    {
        long dim = array.getDim();
        const Coordinates &arrayShape = array.refShape();
        const Coordinates &kernelShape = kernel.refShape();
        const Coordinates &destShape = dest.refShape();

        T *pDestData = dest.getDataPointer();
        const T *pArrayData = array.readDataPointer();
        const T *pKernelData = kernel.readDataPointer();

        for (long i = 0; i < dim; i++)
        {
            if (settings.convAxes[i])
            {
                pArrayData += kernelPos[i] * array.refStrides()[i];
                pKernelData += kernelPos[i] * kernel.refStrides()[i];
            }
        }

        Coordinates c(dim, 0);

        bool end = false;
        while (!end)
        {
            end = true;
            for (long i = 0; i < dim; i++)
            {
                *pDestData += *pArrayData * *pKernelData;

                c[i]++;

                if (i == settings.innerProductAxis)
                {
                    if (c[i] == kernelShape[i])
                    {
                        c[i] = 0;
                        pArrayData -= array.refStrides()[i] * (kernelShape[i] - 1);
                        pKernelData -= kernel.refStrides()[i] * (kernelShape[i] - 1);
                    }
                    else
                    {
                        pArrayData += array.refStrides()[i];
                        pKernelData += kernel.refStrides()[i];
                        end = false;
                        break;
                    }
                }
                else if (settings.convAxes[i])
                {
                    if (c[i] == movingShape[i])
                    {
                        c[i] = 0;
                        pArrayData -= array.refStrides()[i] * (movingShape[i] - 1);
                        pDestData -= kernel.refStrides()[i] * (movingShape[i] - 1);
                    }
                    else
                    {
                        pArrayData += array.refStrides()[i];
                        pDestData += kernel.refStrides()[i];
                        end = false;
                        break;
                    }
                }
                else
                {
                    if (c[i] == movingShape[i])
                    {
                        c[i] = 0;
                        pDestData -= dest.refStrides()[i] * (movingShape[i] - 1);
                        pArrayData -= array.refStrides()[i] * (movingShape[i] - 1);
                        pKernelData -= kernel.refStrides()[i] * (movingShape[i] - 1);
                    }
                    else
                    {
                        pDestData += dest.refStrides()[i];
                        pArrayData += array.refStrides()[i];
                        pKernelData += kernel.refStrides()[i];
                        end = false;
                        break;
                    }
                }
            }
        }
    }

    template <DataType T>
    Array<T> &simpleConvolve(const Array<T> &dest, const Array<T> &array, const Array<T> &kernel, const Settings &settings)
    {
        dest = 0;

        long dim = array.getDim();
        const Coordinates &kernelShape = kernel.refShape();
        Settings s(settings);
        s.padding = Padding::SHRINK;
        const Coordinates movingShape = convShapePostExpansion(array.refShape(), kernel.refShape(), s);

        Coordinates kernelPos(dim, 0);

        bool end = false;
        while (!end)
        {

            end = true;
            for (long i = 0; i < dim; i++)
            {
                if (!settings.convAxes[i])
                    continue;
                else if (kernelPos[i] == kernelShape[i] - 1)
                    kernelPos[i] = 0;
                else
                {
                    kernelPos[i]++;
                    end = false;
                    break;
                }
            }
        }
    }

    template <DataType T>
    Array<T> convolve(const Array<T> &array, const Array<T> &kernel, const Settings &settings)
    {
        Coordinates newShape = convShapePostExpansion(array.refShape(), kernel.refShape(), settings, true);
    }
}

#endif
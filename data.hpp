#ifndef DATA_H
#define DATA_H

#include <vector>

template <typename T>
class Array;

template <typename T>
class Data
{
    friend class Array<T>;
private:
    class Control
    {
        friend class Data<T>;

    private:
        T *mRaw;
        size_t mAccessCount;
        size_t mSize;
        bool mRealeased;

        void release()
        {
            assertm(mAccessCount != 0, "Data has already been fully released!");
            mAccessCount--;

            if (mAccessCount == 0)
            {
                delete[] mRaw;
                mRealeased = true;
            }
        }

    public:
        Control(T *raw, size_t size)
        {
            mRaw = raw;
            mSize = size;
            mAccessCount = 1;
            mRealeased = false;
        }

        Control(const Control &other) = delete;
        Control &operator=(const Control &other) = delete;
        Control(Control &&other) = delete;
        Control &operator=(Control &&other) = delete;
        Control() = delete;

        ~Control()
        {
            assertm(mRealeased, "Data has not been fully released!");
        }
    };

    static_assert(std::is_arithmetic_v<T>, "T of Array<T> must be arithmetic type!");

    // Data<T> does not own the data, Control T does
private:
    T *mRaw;
    Control *mControl;

public:
    size_t size() const { return mControl->mSize; }

    // This constructor does not put initial values in the array, use caution!
    Data<T>(size_t size)
    {
        mRaw = new T[size];
        mControl = new Control(mRaw, size);
    }

    Data<T>(T *raw, size_t size)
    {
        mRaw = raw;
        mControl = new Control(raw, size);
    }

    Data<T>(std::vector<T> &data)
    {
        mRaw = new T[data.size()];
        for (size_t i = 0; i < data.size(); i++)
            mRaw[i] = data[i];
        mControl = new Control(mRaw, data.size());
    }

    Data<T>(const std::initializer_list<T> &values)
    {
        mRaw = new T[values.size()];
        size_t i = 0;
        for (const T &value : values)
            mRaw[i++] = value;
        mControl = new Control(mRaw, values.size());
    }

    Data<T>(const Data<T> &other)
    {
        mRaw = other.mRaw;
        mControl = other.mControl;
        mControl->mAccessCount++;
    }

    Data<T>(Data<T> &&other)
    {
        mRaw = other.mRaw;
        mControl = other.mControl;
        mControl->mAccessCount++;
    }

    Data<T> &operator=(const Data<T> &other)
    {
        if (this == &other)
            return *this;

        mControl->release();
        mRaw = other.mRaw;
        mControl = other.mControl;
        mControl->mAccessCount++;
        return *this;
    }

    Data<T> &operator=(Data<T> &&other)
    {
        if (this == &other)
            return *this;

        mControl->release();
        mRaw = other.mRaw;
        mControl = other.mControl;
        mControl->mAccessCount++;
        return *this;
    }

    Data<T> &operator=(const T value)
    {
        for (size_t i = 0; i < size(); i++)
            mRaw[i] = value;

        return *this;
    }

    Data<T> copy() const
    {
        size_t s = size();
        T *newRaw = new T[s];
        std::copy(mRaw, mRaw + size(), newRaw);
        return Data<T>(newRaw, s);
    }

    T &operator[](long i) const { 
        assertm(i >= 0 && i < size(), "Index out of bounds!");
        return mRaw[i]; }

    void release()
    {
        assertm(mControl != nullptr, "This Data pointer has already released its data!");
        mControl->release();
        if (mControl->mRealeased)
            delete mControl;
        mControl = nullptr;
        mRaw = nullptr;
    }

    ~Data()
    {
        if (mControl != nullptr)
            release();
    }
};

#endif
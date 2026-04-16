#ifndef TEST_ARITHMETICS_H
#define TEST_ARITHMETICS_H

#include "test_util.hpp"

using namespace ArrayLibrary;

namespace Test::Arithmetic
{
    void basic()
    {
        // Create two 1D arrays of length 3 filled with constants
        Array<float> a = Array<float>::constant({3}, 1.0f); // [1, 1, 1]
        Array<float> b = Array<float>::constant({3}, 2.0f); // [2, 2, 2]

        Array<float> c = a + b; // expected [3, 3, 3]
        for (int i = 0; i < c.getFlatLength(); ++i)
        {
            assert(c.getFlat(i) == 3.0f);
        }

        c = a - b; // expected [-1, -1, -1]
        for (int i = 0; i < c.getFlatLength(); ++i)
        {
            assert(c.getFlat(i) == -1.0f);
        }

        c = a * b; // expected [2, 2, 2]
        for (int i = 0; i < c.getFlatLength(); ++i)
        {
            assert(c.getFlat(i) == 2.0f);
        }

        c = a / b; // expected [0.5, 0.5, 0.5]
        for (int i = 0; i < c.getFlatLength(); ++i)
        {
            assert(c.getFlat(i) == 0.5f);
        }

        std::cout << "Basic test passed.\n";
    }

    void broadcast()
    {
        int k = 12;
        Array<float> a = Array<float>::range(k).reshape(1, k);
        Array<float> b = Array<float>::range(k).reshape(k, 1);

        auto c = a - b;

        for (ShapeIterator it(c.refShape()); !it.isFinished(); ++it)
        {
            assert(c[it.refPosition()] == it.refPosition()[1] - it.refPosition()[0]);
        }

        std::cout << "Broadcast test passed.\n";
    }

}

#endif
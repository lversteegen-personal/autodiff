
#ifndef TEST_UNIVERSAL_OPERATIONS_H
#define TEST_UNIVERSAL_OPERATIONS_H

#include "test_util.hpp"

using namespace ArrayLibrary;

namespace Test
{
    void combexConvo()
    {
        const float lam = 0.5;
        auto x = Array<float>({1, 2, 3, 4, 5, 6, 7, 8, 9});
        auto y = Array<float>({1, 4, 9, 16, 25, 36, 49, 64, 81});

        auto z = UniversalPointwise<float, float, float>::compute<ConvexCombination<float>, true, true>(lam, x, y);

        for (long i = 0; i < x.getFlatLength(); i++)
            assert(approxEqual(z.getFlat(i), lam * x.getFlat(i)+ (1-lam)*y.getFlat(i)));
    }
}

#endif
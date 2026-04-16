#ifndef TEST_MATMUL_H
#define TEST_MATMUL_H

#include "test_util.hpp"

namespace Test::Matmul
{

    void matmulSmall()
    {
        auto A = Array<float>({1, 1, -1, 1}).reshape(2, 2);
        auto B = Array<float>({1, 2, 3, 4}).reshape(2, 2);
        auto C = ArrayLibrary::Matmul::matmul<float>(A, B);

        for (int i = 0; i < 2; i++)
        {
            for (int k = 0; k < 2; k++)
            {
                float sum = 0;
                for (int j = 0; j < 2; j++)
                {
                    sum += A.get({i, j}) * B.get({j, k});
                }
                TEST_LOG(approxEqual(C.get({i, k}), sum), std::format("Unexpected result for indices ({},{})", i, k));
            }
        }

        std::cout << "Small matmul test passed.\n";
    }

    void matmul()
    {
        const long m = 67;
        const long p = 41;
        const long n = 100;
        RandomArrayGenerator rng;
        auto A = rng.normal<float>({m, p});
        auto B = rng.normal<float>({p, n});
        auto C = ArrayLibrary::Matmul::matmul<float>(A, B);

        for (int i = 0; i < m; i++)
        {
            for (int k = 0; k < n; k++)
            {
                float sum = 0;
                for (int j = 0; j < p; j++)
                {
                    sum += A.get({i, j}) * B.get({j, k});
                }
                TEST_LOG(approxEqual(C.get({i, k}), sum), std::format("Unexpected result for indices ({},{})", i, k));
            }
        }

        std::cout << "Matmul test passed.\n";
    }

    void matmulOuter()
    {
        const long m = 67;
        const long p = 1;
        const long n = 100;
        RandomArrayGenerator rng;
        auto A = rng.normal<float>({m, p});
        auto B = rng.normal<float>({p, n});
        auto C = ArrayLibrary::Matmul::matmul<float>(A, B);

        for (int i = 0; i < m; i++)
        {
            for (int k = 0; k < n; k++)
            {
                float sum = 0;
                for (int j = 0; j < p; j++)
                {
                    sum += A.get({i, j}) * B.get({j, k});
                }
                TEST_LOG(approxEqual(C.get({i, k}), sum), std::format("Unexpected result for indices ({},{})", i, k));
            }
        }

        std::cout << "Outer matmul test passed.\n";
    }

    void matvecmul()
    {
        const long m = 67;
        const long p = 100;
        const long n = 1;
        RandomArrayGenerator rng;
        auto A = rng.normal<float>({m, p});
        auto B = rng.normal<float>({p, n});
        auto C = ArrayLibrary::Matmul::matmul<float>(A, B);

        for (int i = 0; i < m; i++)
        {
            for (int k = 0; k < n; k++)
            {
                float sum = 0;
                for (int j = 0; j < p; j++)
                {
                    sum += A.get({i, j}) * B.get({j, k});
                }
                TEST_LOG(approxEqual(C.get({i, k}), sum), std::format("Unexpected result for indices ({},{})", i, k));
            }
        }

        std::cout << "Matvecmul test passed.\n";
    }

    void matvecmulSmall()
    {
        const long m = 2;
        const long p = 2;
        const long n = 1;
        auto A = Array<float>({1, 2, 3, 4}).reshape(2, 2);
        auto B = Array<float>({1, 5}).reshape(2, 1);
        auto C = ArrayLibrary::Matmul::matmul<float>(A, B);

        for (int i = 0; i < m; i++)
        {
            for (int k = 0; k < n; k++)
            {
                float sum = 0;
                for (int j = 0; j < p; j++)
                {
                    sum += A.get({i, j}) * B.get({j, k});
                }
                TEST_LOG(approxEqual(C.get({i, k}), sum), std::format("Unexpected result for indices ({},{})", i, k));
            }
        }

        std::cout << "Small matvecmul test passed.\n";
    }

    void all()
    {
        matmulSmall();
        matmul();
        matmulOuter();
        matvecmulSmall();
        matvecmul();
    }
}

#endif
////
//// Created by MRV on 27.02.2023.
////
#include <gtest/gtest.h>
#include "math/solver.hpp"
#include "utils/private_cracker.hpp"
#include <array>

namespace math {
    namespace {
        CreateInvoker(
                Solver<Method::Gauss>,
                Normalize,
                void (Solver<Method::Gauss>::*)(size_t),
                solver_gauss_normalize)
        CreateInvoker(
                Solver<Method::Gauss>,
                LazySublines,
                void (Solver<Method::Gauss>::*)(size_t, size_t),
                solver_gauss_lazy_sublines)
    }
}

namespace {
    math::Matrix TestMatrix(){
        math::Matrix::Arr arr{
                {1, 2, 3},
                {4, 5, 7},
                {10, 4, 3}
        };
        math::Matrix::Col free{
            1,
            2,
            3
        };
        return math::Matrix{.arr_ = std::move(arr), .free_col = std::move(free), .n = 3};
    }
}

bool EqualDoubles(double left, double right){
    auto value = abs(right - left);
    return  value < 0.00000001;
}

void AssertMatrix(const math::Matrix& matrix, const math::Matrix::Arr& exp_arr, const math::Matrix::Col& free_col){
    for (auto i = 0; i < matrix.n; i++){
        for (auto j = 0; j < matrix.n; j++){
            ASSERT_TRUE(EqualDoubles(matrix.arr_[i][j], exp_arr[i][j]));
        }
        ASSERT_TRUE(EqualDoubles(matrix.free_col[i], free_col[i]));
    }
}

TEST(BaseTests, NormalizeTest){
    math::Solver<math::Method::Gauss> solver{TestMatrix()};
    math::crack_use_solver_gauss_normalize(solver, 1);
    std::array<double, 3> exp{4.0/5, 1.0, 7.0/5.0};
    double free_col_exp = 2.0/5.0;
    auto& row = solver.GetMatrix().arr_[1];
    auto& free_col = solver.GetMatrix().free_col[1];
    for (auto i = 0; i < 3; ++i) ASSERT_TRUE(EqualDoubles(row[i],exp[i]));
    ASSERT_TRUE(EqualDoubles(free_col,free_col_exp));
}
TEST(BaseTests, LazySublines){
    math::Solver<math::Method::Gauss> solver{TestMatrix()};
    math::crack_use_solver_gauss_lazy_sublines(solver, 0, 1);
    math::crack_use_solver_gauss_lazy_sublines(solver, 0, 2);
    math::Matrix::Arr exp_arr{
            {1, 2, 3},
            {0, 5 - 4.0*2.0, 7 - 4*3},
            {0, 4 - 10 * 2, 3 - 10 * 3}
    };
    math::Matrix::Col exp_col{
        1, 2 - 4,  3 - 10
    };
    AssertMatrix(solver.GetMatrix(), exp_arr, exp_col);
}
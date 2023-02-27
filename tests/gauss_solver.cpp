////
//// Created by MRV on 27.02.2023.
////
#include <gtest/gtest.h>
#include "math/solver.hpp"
#include "utils/private_cracker.hpp"
#include <array>

CreateGetter(
        math::Solver<math::Method::Gauss>,
        matrix_,
        math::Matrix math::Solver<math::Method::Gauss>::*,
        solver_gauss_Matrix_)

CreateInvoker(
        math::Solver<math::Method::Gauss>,
        Normalize,
        void (math::Solver<math::Method::Gauss>::*)(size_t),
        solver_gauss_Normalize)
CreateInvoker(
        math::Solver<math::Method::Gauss>,
        LazySublinesDown,
        void (math::Solver<math::Method::Gauss>::*)(size_t, size_t),
        solver_gauss_LazySublinesDown)

CreateInvoker(
        math::Solver<math::Method::Gauss>,
        ResetColumnDown,
        void (math::Solver<math::Method::Gauss>::*)(size_t),
        solver_gauss_ResetColumnDown)

CreateInvoker(
        math::Solver<math::Method::Gauss>,
        TransformUpTriangle,
        void (math::Solver<math::Method::Gauss>::*)(),
        solver_gauss_TransformUpTriangle)

CreateInvoker(
        math::Solver<math::Method::Gauss>,
        LazySublinesUp,
        void (math::Solver<math::Method::Gauss>::*)(size_t, size_t),
        solver_gauss_LazySublinesUp)

CreateInvoker(
        math::Solver<math::Method::Gauss>,
        ResetColumnUp,
        void (math::Solver<math::Method::Gauss>::*)(size_t),
        solver_gauss_ResetColumnUp)

CreateInvoker(
        math::Solver<math::Method::Gauss>,
        TransformDownTriangle,
        void (math::Solver<math::Method::Gauss>::*)(),
        solver_gauss_TransformDownTriangle)

decltype(auto) GetMatrix(const math::Solver<math::Method::Gauss>& solver){
    return crack_get_solver_gauss_Matrix_(solver);
}

namespace {
    math::Matrix TestFullMatrix(){
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
    math::Matrix TestZeroFirstColumnNormSecondLineMatrix(){
        math::Matrix::Arr arr{
                {1, 2, 3},
                {0, 1, 5/3.0},
                {0, -16, -27}
        };
        math::Matrix::Col free{
                1, 2/3.0,  -7
        };
        return math::Matrix{.arr_ = std::move(arr), .free_col = std::move(free), .n = 3};
    }
    math::Matrix TestUpTriangleMatrix(){
        math::Matrix::Arr arr{
                {1, 2, 3},
                {0, 1, 5/3.0},
                {0, 0, 1}
        };
        math::Matrix::Col free{
                1, 2/3.0,  -11
        };
        return math::Matrix{.arr_ = std::move(arr), .free_col = std::move(free), .n = 3};
    }
}

bool EqualDoubles(double left, double right){
    auto value = abs(right - left);
    return  value < 0.00000001;
}

std::string PerformMessage(const math::Matrix& matrix,
                           const math::Matrix::Arr& exp_arr){
    auto msg = std::string("Got matrix: \n");
    for (auto& row: matrix.arr_){
        for (auto elem : row){
            msg.append(std::to_string(elem)).append(" ");
        }
        msg.append("\n");
    }
    msg.append("Expected matrix: ");
    for (auto& row: exp_arr){
        for (auto elem : row){
            msg.append(std::to_string(elem)).append(" ");
        }
        msg.append("\n");
    }
    return msg;
}
std::string PerformMessage(const math::Matrix& matrix,
                           const math::Matrix::Col& exp_free_col){
    auto msg = std::string("got free col: ");
    for (auto elem : matrix.free_col){
        msg.append(std::to_string(elem)).append("\n");
    }
    msg.append("exp free col: ");
    for (auto elem : exp_free_col){
        msg.append(std::to_string(elem)).append("\n");
    }
    return msg;
}

void AssertMatrix(const math::Matrix& matrix, const math::Matrix::Arr& exp_arr, const math::Matrix::Col& free_col){
    for (auto i = 0; i < matrix.n; i++){
        for (auto j = 0; j < matrix.n; j++){
            ASSERT_TRUE(EqualDoubles(matrix.arr_[i][j], exp_arr[i][j])) << PerformMessage(matrix, exp_arr);
        }
        ASSERT_TRUE(EqualDoubles(matrix.free_col[i], free_col[i])) << PerformMessage(matrix, free_col);
    }
}

TEST(SolverGaussTests, NormalizeTest){
    math::Solver<math::Method::Gauss> solver{TestFullMatrix()};
    crack_invoke_solver_gauss_Normalize(solver, 1);
    std::array<double, 3> exp{4.0/5, 1.0, 7.0/5.0};
    double free_col_exp = 2.0/5.0;
    auto& row = GetMatrix(solver).arr_[1];
    auto& free_col = GetMatrix(solver).free_col[1];
    for (auto i = 0; i < 3; ++i) ASSERT_TRUE(EqualDoubles(row[i],exp[i]));
    ASSERT_TRUE(EqualDoubles(free_col,free_col_exp));
}
TEST(SolverGaussTests, LazySublinesDown){
    math::Solver<math::Method::Gauss> solver{TestFullMatrix()};
    crack_invoke_solver_gauss_LazySublinesDown(solver, 0, 1);
    math::Matrix::Arr exp_arr{
            {1, 2, 3},
            {0, -3, -5},
            {10, 4, 3}
    };
    math::Matrix::Col exp_col{
        1, -2, 3
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_col);
}

TEST(SolverGaussTests, ResetColumnDown){
    math::Solver<math::Method::Gauss> solver{TestFullMatrix()};
    crack_invoke_solver_gauss_ResetColumnDown(solver, 0);
    math::Matrix::Arr exp_arr{
            {1, 2, 3},
            {0, -3, -5},
            {0, -16, -27}
    };
    math::Matrix::Col exp_col{
            1, -2,  -7
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_col);
}

TEST(SolverGaussTests, ResetColumnDown2){
    math::Solver<math::Method::Gauss> solver{TestZeroFirstColumnNormSecondLineMatrix()};
    crack_invoke_solver_gauss_ResetColumnDown(solver, 1);
    math::Matrix::Arr exp_arr{
            {1, 2, 3},
            {0, 1, 5/3.0},
            {0, 0, -27 + 16 * 5/3.0}
    };
    math::Matrix::Col exp_free{
            1, 2/3.0,  -7 + 32/3.0
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_free);
}


TEST(SolverGaussTests, TransformUpTriangle){
    math::Solver<math::Method::Gauss> solver{TestFullMatrix()};
    crack_invoke_solver_gauss_TransformUpTriangle(solver);
    math::Matrix::Arr exp_arr{
            {1, 2, 3},
            {0, 1, 5/3.0},
            {0, 0, 1}
    };
    math::Matrix::Col exp_free{
            1, 2/3.0,  -11
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_free);
}

TEST(SolverGaussTests, LazySublinesUp){
    math::Solver<math::Method::Gauss> solver{TestUpTriangleMatrix()};
    crack_invoke_solver_gauss_LazySublinesUp(solver, 2, 1);
    math::Matrix::Arr exp_arr{
            {1, 2, 3},
            {0, 1, 0},
            {0, 0, 1}
    };
    math::Matrix::Col exp_free{
            1, 57/3.0,  -11
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_free);
}

TEST(SolverGaussTests, ResetColumnUp){
    math::Solver<math::Method::Gauss> solver{TestUpTriangleMatrix()};
    crack_invoke_solver_gauss_ResetColumnUp(solver, 2);
    math::Matrix::Arr exp_arr{
            {1, 2, 0},
            {0, 1, 0},
            {0, 0, 1}
    };
    math::Matrix::Col exp_free{
            34, 57/3.0,  -11
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_free);
}

TEST(SolverGaussTests, Solve){
    math::Solver<math::Method::Gauss> solver{TestFullMatrix()};
    solver.GetSolution();
    math::Matrix::Arr exp_arr{
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
    };
    math::Matrix::Col exp_free{
        -4, 57/3.0, -11
    };
    AssertMatrix(GetMatrix(solver), exp_arr, exp_free);
}


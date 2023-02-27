#include <gtest/gtest.h>
#include "math/solver.hpp"
#include "utils/private_cracker.hpp"
#include <array>

CreateGetter(
        math::Solver<math::Method::Rotations>,
        matrix_,
        math::Matrix math::Solver<math::Method::Rotations>::*,
        solver_rotations_Matrix_)

CreateInvoker(
        math::Solver<math::Method::Rotations>,
        LazySublinesUp,
        void (math::Solver<math::Method::Rotations>::*)(size_t, size_t),
        solver_rotations_LazySublinesUp)

CreateInvoker(
        math::Solver<math::Method::Rotations>,
        ResetColumnUp,
        void (math::Solver<math::Method::Rotations>::*)(size_t),
        solver_rotations_ResetColumnUp)

CreateInvoker(
        math::Solver<math::Method::Rotations>,
        TransformUpTriangle,
        void (math::Solver<math::Method::Rotations>::*)(),
        solver_rotations_TransformUpTriangle)

inline decltype(auto) GetMatrix(const math::Solver<math::Method::Rotations>& solver){
    return crack_get_solver_rotations_Matrix_(solver);
}

namespace {
    inline math::Matrix TestFullMatrix(){
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
    inline math::Matrix TestZeroFirstColumnNormSecondLineMatrix(){
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
    inline math::Matrix TestUpTriangleMatrix(){
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

inline bool EqualDoubles(double left, double right){
    auto value = abs(right - left);
    return  value < 0.00000001;
}

inline bool IsTriangleUpMatrix(const math::Matrix& matrix){
    for (size_t i = 0; i < matrix.n; ++i){
        for (size_t j = 0; j < matrix.n; ++j){
            if (j < i && !EqualDoubles(0, matrix.arr_[i][j])){
                return false;
            }
        }
    }
    return true;
}

inline std::string PerformMessage(const math::Matrix& matrix,
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
inline std::string PerformMessage(const math::Matrix& matrix,
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

inline void AssertMatrix(const math::Matrix& matrix, const math::Matrix::Arr& exp_arr, const math::Matrix::Col& free_col){
    for (auto i = 0; i < matrix.n; i++){
        for (auto j = 0; j < matrix.n; j++){
            ASSERT_TRUE(EqualDoubles(matrix.arr_[i][j], exp_arr[i][j])) << PerformMessage(matrix, exp_arr);
        }
        ASSERT_TRUE(EqualDoubles(matrix.free_col[i], free_col[i])) << PerformMessage(matrix, free_col);
    }
}


TEST(SolverRotationsTests, LazySublinesUp){
    math::Solver<math::Method::Rotations> solver{TestUpTriangleMatrix()};
    crack_invoke_solver_rotations_LazySublinesUp(solver, 2, 1);
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

TEST(SolverRotationsTests, ResetColumnUp){
    math::Solver<math::Method::Rotations> solver{TestUpTriangleMatrix()};
    crack_invoke_solver_rotations_ResetColumnUp(solver, 2);
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

TEST(SolverRotationsTests, TransformUpTriangle){
    math::Solver<math::Method::Rotations> solver{TestFullMatrix()};
    crack_invoke_solver_rotations_TransformUpTriangle(solver);
    auto& matrix = GetMatrix(solver);
    ASSERT_TRUE(IsTriangleUpMatrix(matrix));
}


TEST(SolverRotationsTests, Solve){
    math::Solver<math::Method::Rotations> solver{TestFullMatrix()};
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


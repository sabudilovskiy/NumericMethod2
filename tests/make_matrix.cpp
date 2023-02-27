#include <gtest/gtest.h>
#include "math/solver.hpp"
#include "utils/private_cracker.hpp"
#include <sstream>
#include <array>

namespace {
    auto BaseTest(){
        return std::stringstream{
            "3\n"
            "1 2 3\n"
            "4 5 6\n"
            "7 8 9 \n"
            "10 11 12"
        };
    }
    auto EmptyMatrixTest(){
        return std::stringstream{
                "3\n"
        };
    }
    auto NonSquareMatrixTest(){
        return std::stringstream{
                "3\n"
                "1 2 3 13\n"
                "4 5 6 16\n"
                "7 8 9 19 \n"
                "10 11 12"
        };
    }
    auto EmptyTest(){
        return std::stringstream{};
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
}

TEST(LoadFromStreamTests, basic){
    auto matrix = math::MakeMatrix(BaseTest());
    math::Matrix::Arr exp_arr{
            {1,2,3},
            {4,5,6},
            {7,8,9}
    };
    math::Matrix::Col exp_free{
        10,11,12
    };
    AssertMatrix(matrix, exp_arr, exp_free);
}

TEST(LoadFromStreamTests, EmptyMatrix){
    ASSERT_THROW(math::MakeMatrix(EmptyMatrixTest()), std::runtime_error);
}
TEST(LoadFromStreamTests, Empty){
    ASSERT_THROW(math::MakeMatrix(EmptyTest()), std::runtime_error);
}
TEST(LoadFromStreamTests, NonSquare){
    ASSERT_THROW(math::MakeMatrix(NonSquareMatrixTest()), std::runtime_error);
}
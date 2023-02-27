#ifndef NUMERIC_METHODS2_MATH_SOLVER
#define NUMERIC_METHODS2_MATH_SOLVER
#include <iostream>
#include <type_traits>
#include "matrix.hpp"
#include <cassert>
#include <cmath>

namespace math {

    enum class Method {
        Gauss, Rotations
    };

    template<Method method>
    class Solver;

    template<>
    class Solver<Method::Gauss> final {
    protected:
        void Normalize(size_t index) {
            auto &row = matrix_.arr_[index];
            auto& free_col = matrix_.free_col;
            for (size_t i = 0; i < matrix_.n; ++i) {
                assert(row[index] != 0);
                if (i != index) row[i] /= row[index];
            }
            free_col[index] /= row[index];
            row[index] = 1;
        }

        void LazySublinesDown(size_t from, size_t to) {
            auto &from_line = matrix_.arr_[from];
            auto &to_line = matrix_.arr_[to];
            auto cof = to_line[from];
            for (size_t index = from + 1; index < matrix_.n; index++) {
                to_line[index] -= from_line[index] * cof;
            }
            to_line[from] = 0;
            matrix_.free_col[to] -=  matrix_.free_col[from] * cof;
        }

        void ResetColumnDown(size_t index_col) {
            for (size_t i = index_col + 1; i < matrix_.n; ++i) {
                LazySublinesDown(index_col, i);
            }
        }


        void TransformUpTriangle() {
            for (size_t cur = 0; cur < matrix_.n; ++cur) {
                Normalize(cur);
                ResetColumnDown(cur);
            }
        }

        void LazySublinesUp(size_t from, size_t to) {
            auto &to_line = matrix_.arr_[to];
            auto cof = to_line[from];
            to_line[from] = 0;
            matrix_.free_col[to] -=  matrix_.free_col[from] * cof;
        }
        void ResetColumnUp(size_t index_col) {
            for (size_t i = index_col; i > 0; --i) {
                LazySublinesUp(index_col, i - 1);
            }
        }
        void TransformDownTriangle() {
            for (size_t cur = matrix_.n; cur > 0; --cur) {
                ResetColumnUp(cur - 1);
            }
        }
        void Solve(){
            TransformUpTriangle();
            TransformDownTriangle();
        }
    public:
        Solver(Matrix matrix) : matrix_(std::move(matrix)) {}
        const Matrix::Col& GetSolution(){
            if (!solved){
                Solve();
                solved = true;
            }
            return matrix_.free_col;
        }
    private:
        bool solved = false;
        Matrix matrix_;
    };
    template<>
    class Solver<Method::Rotations> final {
    protected:

        void LazySublinesUp(size_t from, size_t to) {
            auto &to_line = matrix_.arr_[to];
            auto &from_line = matrix_.arr_[from];
            auto cof = to_line[from]/from_line[from];
            to_line[from] = 0;
            matrix_.free_col[to] -=  matrix_.free_col[from] * cof;
        }
        void ResetColumnUp(size_t index_col) {
            for (size_t i = index_col; i > 0; --i) {
                LazySublinesUp(index_col, i - 1);
            }
        }
        void Rotate(size_t i, size_t j){
            auto& A = matrix_.arr_;
            auto& b = matrix_.free_col;
            auto R = pow(A[i][i], 2) + pow(A[j][i], 2);
            auto c = A[i][i] / sqrt(R);
            auto s = A[j][i] / sqrt(R);
            for (size_t k = 0; k < matrix_.n; ++k){
                auto temp = -s * A[i][k] + c * A[j][k];
                A[i][k] = c * A[i][k] + s * A[j][k];
                A[j][k] = temp;
            }
            auto temp = -s * b[i] + c * b[j];
            b[i] = c*b[i] + s * b[j];
            b[j] = temp;
        }
        void TransformUpTriangle(){
            for (auto i = 0; i < matrix_.n - 1; ++i){
                for (auto j = i + 1 ; j < matrix_.n; ++j){
                    Rotate(i, j);
                }
            }
        }
        void TransformDownTriangle() {
            for (size_t cur = matrix_.n; cur > 0; --cur) {
                ResetColumnUp(cur - 1);
            }
        }
        void Solve(){
            TransformUpTriangle();
            TransformDownTriangle();
            NormalizeAll();
        }
        void Normalize(size_t i){
            matrix_.free_col[i] /= matrix_.arr_[i][i];
            matrix_.arr_[i][i] = 1;
        }
        void NormalizeAll(){
            for (size_t i = 0; i < matrix_.n; ++i){
                Normalize(i);
            }
        }
    public:
        Solver(Matrix matrix) : matrix_(std::move(matrix)) {}
        const Matrix::Col& GetSolution(){
            if (!solved){
                Solve();
                solved = true;
            }
            return matrix_.free_col;
        }
    private:
        bool solved = false;
        Matrix matrix_;
    };
}
#endif
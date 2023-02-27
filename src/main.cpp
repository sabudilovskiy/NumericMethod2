#include <iostream>
#include "math/solver.hpp"
#include "utils/generator.hpp"

template <typename Solution>
void PrintGeneratedSolution(Solution&& solution){
    std::cout << "Сгенерированное решение: \n";
    for (auto& x : solution){
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

template<typename Result>
void PrintGenerateResult(Result&& result){
    const auto& [matrix, free_column, solution] = result;
    std::cout << "Сгенерированная матрица: \n";
    for (auto& row: matrix) {
        for (auto elem : row) std::cout << elem << ' ';
        std::cout << '\n';
    }
    std::cout << "Сгенерированный столбец свободных членов: \n";
    for (auto& elem_column : free_column){
        std::cout << elem_column << '\n';
    }
    PrintGeneratedSolution(solution);
}

template <typename Solution>
void PrintSolution(Solution&& solution){
    std::cout << "Решение: \n";
    for (auto& x : solution){
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

template<typename From>
math::Matrix ToMatrix(From&& from) {
    math::Matrix matrix;
    const auto &[arr, free_column, solution] = from;
    matrix.n = arr.size();
    matrix.arr_.resize(matrix.n);
    matrix.free_col.resize(matrix.n);
    for (size_t i = 0; i < matrix.n; ++i){
        matrix.arr_[i].resize(matrix.n);
        matrix.free_col[i] = free_column[i];
        for (size_t j = 0; j < matrix.n; j++){
            matrix.arr_[i][j] = arr[i][j];
        }
    }
    return matrix;
}

int main() {
    while (true){
        static size_t constexpr k = 3;
        auto checker = [](size_t i, size_t j, size_t size){
            return true;
        };
        constexpr auto range = 10000;
        constexpr auto gen_traits = Traits<double>{
                .kSize = 10,
                .kMin = -range,
                .kMax = range};
        Generator<double, gen_traits, RandomSeed::Yes, std::uniform_real_distribution<double>> generator{checker};
        generator.Generate();
        PrintGenerateResult(generator.GetResult());
        auto [arr, col, _] = generator.GetResult();
        try{
            math::Solver<math::Method::Gauss> solver_gauss{ToMatrix(generator.GetResult())};
            math::Solver<math::Method::Rotations> solver_rotations{ToMatrix(generator.GetResult())};
            std::cout << "Gauss: \n";
            PrintSolution(solver_gauss.GetSolution());
            std::cout << "Rotations: \n";
            PrintSolution(solver_rotations.GetSolution());
        }
        catch (std::exception& exc){
            std::cout << exc.what();
        }
        PrintGeneratedSolution(std::get<2>(generator.GetResult()));
        std::cin.get();
    }
    return 0;
}

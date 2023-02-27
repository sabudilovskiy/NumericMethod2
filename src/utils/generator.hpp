//
// Created by MRV on 19.02.2023.
//

#ifndef NUMERICMETHODS1_GENERATOR_HPP
#define NUMERICMETHODS1_GENERATOR_HPP

#include <random>
#include <array>
#include <functional>

template<typename Number>
struct Traits{
    size_t kSize;
    Number kMin;
    Number kMax;
};

enum struct RandomSeed{
    Yes, No
};

template<typename Number,
        Traits<Number> traits,
        RandomSeed kRandomSeed = RandomSeed::No,
        typename Distribution = std::uniform_int_distribution<Number>>
class Generator final{
public:
    static constexpr inline auto kSize = traits.kSize;
    static constexpr inline auto kMin = traits.kMin;
    static constexpr inline auto kMax = traits.kMax;
    using Row = std::array<Number,kSize>;
    using Matrix = std::array<Row, kSize>;
    using FreeMembersColumn = std::array<Number, kSize>;
    using Solution = std::array<Number, kSize>;
    void Generate(){
        GenerateMatrix();
        GenerateSolution();
        CountFreeMembers();
    }
    template<class Checker>
    Generator(Checker&& checker) : checker_(std::forward<Checker>(checker)){
        static_assert(kMin < kMax, "минимум должен быть больше максимума");
    }
    auto GetResult() const {
        return std::tie(matrix_, free_members_column_, solution_);
    }

protected:
    Number GenerateNumber(){
        auto number = distribution_(number_generator_);
        while (number == 0) number = distribution_(number_generator_);
        return number;
    }
    void GenerateMatrix(){
        for (size_t i = 0; i < kSize; ++i){
            for (size_t j = 0; j < kSize; ++j){
                if (checker_(i, j, kSize)){
                    matrix_[i][j] = GenerateNumber();
                }
                else matrix_[i][j] = 0;
            }
        }
    }
    void GenerateSolution(){
        for (auto& elem: solution_){
            elem = GenerateNumber();
        }
    }
    void CountFreeMembers(){
        for (size_t i = 0; i < kSize; ++i){
            const Row& row = matrix_[i];
            Number sum = {};
            for (size_t j = 0; j < kSize; ++j){
                sum += row[j] * solution_[j];
            }
            free_members_column_[i] = sum;
        }
    }
private:
    Matrix matrix_;
    FreeMembersColumn free_members_column_;
    Solution solution_;
    std::function<bool(size_t i , size_t j, size_t size)> checker_;
    Distribution distribution_{kMin, kMax};
    std::mt19937 number_generator_{kRandomSeed == RandomSeed::Yes ? std::random_device{}() : 0};
};

#endif //NUMERICMETHODS1_GENERATOR_HPP

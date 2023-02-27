#ifndef NUMERIC_METHODS2_MATH_MATRIX
#define NUMERIC_METHODS2_MATH_MATRIX

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace math {
    struct Matrix {
        using Row = std::vector<double>;
        using Arr = std::vector<Row>;
        using Col = std::vector<double>;
        Arr arr_;
        Col free_col;
        size_t n;
    };

    inline void IncompatibleSizeRows(size_t cur) {
        auto msg = std::string{"Не совпадают размеры строк: "}
                .append(std::to_string(cur - 1))
                .append(" и ")
                .append(std::to_string(cur));
        throw std::runtime_error(msg);
    }

    inline void EmptyMatrix() {
        auto msg = std::string{"Матрица нет: "};
        throw std::runtime_error(msg);
    }

    inline void NonSquareMatrix(size_t lines, size_t columns) {
        auto msg = std::string{"Матрица не квадратная: "}
                .append(std::to_string(lines))
                .append(" x ")
                .append(std::to_string(columns));
        throw std::runtime_error(msg);
    }

    inline void IncompatibleSizeFreeColumns(size_t expected, size_t got) {
        auto msg = std::string{"Не совпадают размеры матрицы и столбца свободных членов: "}
                .append(std::to_string(expected))
                .append(" != ")
                .append(std::to_string(got));
        throw std::runtime_error(msg);
    }

    template<typename Stream>
    Matrix MakeMatrix(Stream &&stream) {
        std::string line;
        Matrix matrix;
        auto&[arr_, free_col, n] = matrix;
        size_t cur = 0;
        if (!std::getline(stream, line)){
            EmptyMatrix();
        }
        n = std::stoll(line);
        while (cur < n && std::getline(stream, line)) {
            std::stringstream ss{line};
            Matrix::Row row;
            std::copy(std::istream_iterator<double>(ss), std::istream_iterator<double>(), std::back_inserter(row));
            arr_.push_back(std::move(row));
            if (cur >= 1 && arr_[cur].size() != arr_[cur - 1].size()) {
                IncompatibleSizeRows(cur);
            }
            cur++;
        }
        if (arr_.size() == 0){
            EmptyMatrix();
        }
        if (arr_.size() != arr_[0].size()) {
            NonSquareMatrix(arr_.size(), arr_[0].size());
        }
        std::copy(std::istream_iterator<double>(stream), std::istream_iterator<double>(), std::back_inserter(free_col));
        if (arr_.size() != free_col.size()) {
            IncompatibleSizeFreeColumns(arr_.size(), free_col.size());
        }
        n = cur;
        return matrix;
    }
}
#endif
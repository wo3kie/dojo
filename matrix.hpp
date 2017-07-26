#ifndef __CXX_DOJO_MATRIX_HPP__
#define __CXX_DOJO_MATRIX_HPP__

/*
 * Website:
 *      https://github.com/wo3kie/dojo
 *
 * Author:
 *      Lukasz Czerwinski
 */

#include <iomanip>
#include <vector>

#include "./feq.hpp"
#include "./floatFormatter.hpp"
#include "./output.hpp"

namespace details
{

class Row
{
public:
    Row() = default;
    Row(Row const &) = default;
    Row(Row &&) = default;

    Row & operator=(Row const &) = default;
    Row & operator=(Row &&) = default;

    Row(unsigned size, double init = 0.0)
        : row_(size, init)
    {
    }

    Row(std::initializer_list<double> const & data)
        : row_(data)
    {
    }

    double & operator[](unsigned i){
        return row_.at(i);
    }

    double const & operator[](unsigned i)const{
        return row_.at(i);
    }

    unsigned columns()const{
        return row_.size();
    }

private:
    std::vector<double> row_;
};

}

class Matrix
{
public:
    typedef details::Row Row;

public:
    Matrix() = default;
    Matrix(Matrix const &) = default;
    Matrix(Matrix &&) = default;

    Matrix & operator=(Matrix const &) = default;
    Matrix & operator=(Matrix &&) = default;

    Matrix(unsigned rows, unsigned columns, double init = 0.0)
        : matrix_(rows, Row(columns, init))
    {
    }

    Matrix(std::initializer_list<std::initializer_list<double>> const & data)
        : matrix_(data.begin(), data.end())
    {
    }

    Row & operator[](unsigned i){
        return matrix_.at(i);
    }

    Row const & operator[](unsigned i)const{
        return matrix_.at(i);
    }

    unsigned rows()const{
        return matrix_.size();
    }

    unsigned columns()const{
        if(rows() == 0){
            return 0;
        }

        return matrix_[0].columns();
    }

private:
    std::vector<Row> matrix_;
};

inline
std::ostream & operator<<(std::ostream & out, Matrix const & matrix){
    out << "[";

    if(matrix.rows() != 0){
        out << "[";

        if(matrix[0].columns() != 0){
            out << FloatFormatter(matrix[0][0], 8);

            for(unsigned j = 1; j < matrix[0].columns(); ++j){
                out << ' ' << FloatFormatter(matrix[0][j], 8);
            }
        }

        out << "]";

        for(unsigned i = 1; i < matrix.rows(); ++i){
            out << "\n [";

            if(matrix[i].columns() != 0){
                out << FloatFormatter(matrix[i][0], 8);

                for(unsigned j = 1; j < matrix[i].columns(); ++j){
                    out << ' ' << FloatFormatter(matrix[i][j], 8);
                }
            }

            out << ']';
        }
    }

    out << "]";

    return out;
}

inline
bool operator==(Matrix const & matrix1, Matrix const & matrix2){
    unsigned const rows1 = matrix1.rows();
    unsigned const rows2 = matrix2.rows();

    if(rows1 != rows2){
        return false;
    }

    for(unsigned row = 0; row < rows1; ++row){
        unsigned const columns1 = matrix1[row].columns();
        unsigned const columns2 = matrix2[row].columns();

        if(columns1 != columns2){
            return false;
        }

        for(unsigned column = 0; column < columns1; ++column){
            if(!feq(matrix1[row][column], matrix2[row][column])){
                return false;
            }
        }
    }

    return true;
}

inline
bool operator!=(Matrix const & matrix1, Matrix const & matrix2){
    return !(matrix1 == matrix2);
}

inline
Matrix operator+(Matrix const & matrix1, double d){
    Matrix result = Matrix(matrix1.rows(), matrix1.columns());

    for(unsigned i = 0; i < matrix1.rows(); ++i){
        for(unsigned j = 0; j < matrix1[0].columns(); ++j){
            result[i][j] = matrix1[i][j] + d;
        }
    }

    return result;
}

inline
Matrix operator+(Matrix const & matrix1, Matrix const & matrix2){
    Matrix result = Matrix(matrix1.rows(), matrix1.columns());

    for(unsigned i = 0; i < matrix1.rows(); ++i){
        for(unsigned j = 0; j < matrix1.columns(); ++j){
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    return result;
}

inline
Matrix operator*(Matrix const & matrix1, double d){
    Matrix result = Matrix(matrix1.rows(), matrix1.columns());

    for(unsigned i = 0; i < matrix1.rows(); ++i){
        for(unsigned j = 0; j < matrix1[0].columns(); ++j){
            result[i][j] = matrix1[i][j] * d;
        }
    }

    return result;
}

inline
Matrix operator*(Matrix const & matrix1, Matrix const & matrix2){
    Matrix result = Matrix(matrix1.rows(), matrix1.columns());

    for(unsigned i = 0; i < matrix1.rows(); ++i){
        for(unsigned j = 0; j < matrix2.columns(); ++j){
            for(unsigned k = 0; k < matrix2.rows(); ++k){
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

inline
Matrix transpose(Matrix const & matrix){
    Matrix result = Matrix(matrix.rows(), matrix.columns());

    for(unsigned i = 0; i < matrix.rows(); ++i){
        for(unsigned j = 0; j < matrix.columns(); ++j){
            result[i][j] = matrix[j][i];
        }
    }

    return result;
}

#endif

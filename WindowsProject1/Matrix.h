#pragma once

#include <vector>
#include <stdexcept>


class Matrix {
public:
    std::vector<std::vector<int>> elements;

    Matrix(std::vector<std::vector<int>> list);

    // Scalar multiplication
    Matrix operator*(int scalar) const;

    // Addition
    Matrix operator+(const Matrix& rhs) const;

    // Subtraction
    Matrix operator-(const Matrix& rhs) const;

    // Matrix multiplication
    Matrix operator*(const Matrix& rhs) const;

    // Transpose
    Matrix transpose() const;

    // Determinant
    int determinant() const;
};
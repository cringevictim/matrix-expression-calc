#include "Matrix.h"


Matrix::Matrix(std::vector<std::vector<int>> list) : elements(list) {}

Matrix Matrix::operator*(int scalar) const {
    Matrix result = *this;
    for (auto& row : result.elements) {
        for (auto& element : row) {
            element *= scalar;
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    if (elements.size() != rhs.elements.size() || elements[0].size() != rhs.elements[0].size()) {
        throw std::invalid_argument("Matrices must be the same size.");
    }
    Matrix result = *this;
    for (size_t i = 0; i < elements.size(); ++i) {
        for (size_t j = 0; j < elements[i].size(); ++j) {
            result.elements[i][j] += rhs.elements[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
    if (elements.size() != rhs.elements.size() || elements[0].size() != rhs.elements[0].size()) {
        throw std::invalid_argument("Matrices must be the same size.");
    }
    Matrix result = *this;
    for (size_t i = 0; i < elements.size(); ++i) {
        for (size_t j = 0; j < elements[i].size(); ++j) {
            result.elements[i][j] -= rhs.elements[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    if (elements[0].size() != rhs.elements.size()) {
        throw std::invalid_argument("The number of columns in the first matrix must equal the number of rows in the second.");
    }
    Matrix result(std::vector<std::vector<int>>(elements.size(), std::vector<int>(rhs.elements[0].size(), 0.0)));
    for (size_t i = 0; i < elements.size(); ++i) {
        for (size_t j = 0; j < rhs.elements[0].size(); ++j) {
            for (size_t k = 0; k < elements[0].size(); ++k) {
                result.elements[i][j] += elements[i][k] * rhs.elements[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result(std::vector<std::vector<int>>(elements[0].size(), std::vector<int>(elements.size())));
    for (size_t i = 0; i < elements.size(); ++i) {
        for (size_t j = 0; j < elements[i].size(); ++j) {
            result.elements[j][i] = elements[i][j];
        }
    }
    return result;
}

int Matrix::determinant() const {
    if (elements.size() != elements[0].size()) {
        throw std::invalid_argument("Determinant is only defined for square matrices.");
    }
    int n = elements.size();
    if (n == 1) return elements[0][0];
    if (n == 2) return elements[0][0] * elements[1][1] - elements[0][1] * elements[1][0];

    int det = 0.0;
    for (int p = 0; p < n; p++) {
        Matrix subMatrix(std::vector<std::vector<int>>(n - 1, std::vector<int>(n - 1)));
        for (int i = 1; i < n; i++) {
            for (int j = 0, col = 0; j < n; j++) {
                if (j == p) continue;
                subMatrix.elements[i - 1][col++] = elements[i][j];
            }
        }
        det += elements[0][p] * subMatrix.determinant() * ((p % 2 == 0) ? 1 : -1);
    }
    return det;
}

#include "Vector.h"


Vector::Vector(std::initializer_list<double> list) : components(list) {}

// Addition
Vector Vector::operator+(const Vector & rhs) const {
    if (components.size() != rhs.components.size()) {
        throw std::invalid_argument("Vectors must be the same size.");
    }
    Vector result = *this;
    for (size_t i = 0; i < components.size(); ++i) {
        result.components[i] += rhs.components[i];
    }
    return result;
}

// Subtraction
Vector Vector::operator-(const Vector& rhs) const {
    if (components.size() != rhs.components.size()) {
        throw std::invalid_argument("Vectors must be the same size.");
    }
    Vector result = *this;
    for (size_t i = 0; i < components.size(); ++i) {
        result.components[i] -= rhs.components[i];
    }
    return result;
}

// Scalar multiplication
Vector Vector::operator*(double scalar) const {
    Vector result = *this;
    for (auto& component : result.components) {
        component *= scalar;
    }
    return result;
}

// Dot product
double Vector::dot(const Vector& rhs) const {
    if (components.size() != rhs.components.size()) {
        throw std::invalid_argument("Vectors must be the same size.");
    }
    double result = 0.0;
    for (size_t i = 0; i < components.size(); ++i) {
        result += components[i] * rhs.components[i];
    }
    return result;
}

// Cross product (only for 3-dimensional vectors)
Vector Vector::cross(const Vector& rhs) const {
    if (components.size() != 3 || rhs.components.size() != 3) {
        throw std::invalid_argument("Cross product is only defined for 3-dimensional vectors.");
    }
    return Vector{
        components[1] * rhs.components[2] - components[2] * rhs.components[1],
        components[2] * rhs.components[0] - components[0] * rhs.components[2],
        components[0] * rhs.components[1] - components[1] * rhs.components[0]
    };
}

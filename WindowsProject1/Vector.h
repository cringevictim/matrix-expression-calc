#pragma once

#include <vector>
#include <stdexcept>


class Vector {
public:
    std::vector<double> components;

    Vector(std::initializer_list<double> list);

    // Addition
    Vector operator+(const Vector& rhs) const;


    // Subtraction
    Vector operator-(const Vector& rhs) const;

    // Scalar multiplication
    Vector operator*(double scalar) const;

    // Dot product
    double dot(const Vector& rhs) const;

    // Cross product (only for 3-dimensional vectors)
    Vector cross(const Vector& rhs) const;
};
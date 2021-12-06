#include "vec2.h"
#include <math.h>
#include <stdio.h>

/// Create a 2d vector.
/// @param x_ The first component.
/// @param y_ The second component.
/// @return The newly created vector.
vec2 vec2_create(double x_, double y_) {
    vec2 vector = {.x = x_, .y = y_};
    return vector;
}

/// Create a zero 2d vector.
/// @return The newly created zero vector.
vec2 vec2_create_zero() { vec2_create(0.0, 0.0); }

/// Add two vectors.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @return The sum in a new vector.
vec2 vec2_add(vec2 lhs, vec2 rhs) {
    vec2 vector = {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
    return vector;
}

/// Substract two vectors.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @return The difference in a new vector.
vec2 vec2_sub(vec2 lhs, vec2 rhs) {
    vec2 vector = {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
    return vector;
}

/// Multiply a vector by a scalar.
/// @param scalar The left operand, a scalar.
/// @param rhs The right operand, a vector.
/// @return The product in a new vector.
vec2 vec2_mul(double scalar, vec2 rhs) {
    vec2 vector = {.x = scalar * rhs.x, .y = scalar * rhs.y};
    return vector;
}

/// Compute the dot product (scalar product) between two vectors.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @return The dot product.
double vec2_dot(vec2 lhs, vec2 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/// Compute the square of the euclidean norm of a given vector.
/// @param v The vector.
/// @return The square of the norm.
double vec2_norm_sqr(vec2 v) {
    return pow(v.x, 2) + pow(v.y, 2);
}

/// Compute the euclidean norm of a given vector.
/// @param v The vector.
/// @return The norm.
double vec2_norm(vec2 v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

/// Compute the normalization of a given vector.
/// @param v The vector.
/// @return The new normalized vector.
vec2 vec2_normalize(vec2 v) {
    double norm = vec2_norm(v);
    vec2 vector = {.x = v.x / norm, .y = v.y / norm};
    return vector;
}

/// Check whether two vectors are approximately equals within a given tolerance.
/// @param lhs The left operand.
/// @param rhs The right operand.
/// @param eps The tolerance.
/// @return The dot product.
bool vec2_is_approx_equal(vec2 lhs, vec2 rhs, double eps) {
    return vec2_norm(vec2_sub(lhs, rhs)) < eps;
}

/// Compute the coordinates of a 2d vector (with components between 0 and 1)
/// in a given screen matrix.
/// @param v The 2d vector.
/// @param width The screen width.
/// @param height The screen height.
/// @return The coordinates (rwo, column).
coordinates vec2_to_coordinates(vec2 v, uint32_t width, uint32_t height) {
    return (coordinates){
            .row = (uint32_t)round((v.y + 1.0) * (height - 1) / 2.0),
            .column = (uint32_t)round((v.x + 1.0) * (width - 1) / 2.0)};
}

/// Print a vector in the standard output.
/// @param v The vector.
void vec2_print(vec2 v)
{
    printf("x = %g, y = %g\n", v.x, v.y);
}

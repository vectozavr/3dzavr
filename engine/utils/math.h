//
// Created by Neirokan on 14.02.2024.
//

#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#include <cstdint>

/**
 * Calculates the base-2 logarithm of a 64-bit unsigned integer.
 *
 * This function computes the logarithm base 2 of the given 64-bit unsigned integer value. It is designed to be efficient
 * for use in scenarios where the precise calculation of log2 is not critical, but the computational efficiency is very important.
 * The function returns an integer representing the floor of the logarithm.
 *
 * @param value The 64-bit unsigned integer for which to calculate the base-2 logarithm.
 * @return The base-2 logarithm of the input value as an integer.
 */
int log2_u64(uint64_t value);

#endif //UTILS_MATH_H

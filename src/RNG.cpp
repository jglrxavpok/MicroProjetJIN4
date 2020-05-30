//
// Created by jglrxavpok on 30/05/2020.
//

#include "RNG.h"

RNG::RNG(long seed): generator(seed), distribution(0.0f, 1.0f) {}

float RNG::rand(float min, float max) {
    return distribution(generator) * (max-min) + min;
}


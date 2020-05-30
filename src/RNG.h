//
// Created by jglrxavpok on 30/05/2020.
//

#pragma once
#include <random>
#include <chrono>

class RNG {
private:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

public:
    explicit RNG(long seed = std::chrono::system_clock::now().time_since_epoch().count());
    float rand(float min = 0.0f, float max = 1.0f);
};

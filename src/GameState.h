//
// Created by jglrxavpok on 02/06/2020.
//

#pragma once

class GameState {
private:
    float playerLife;
    float maxPlayerLife;

public:
    explicit GameState(float maxPlayerLife);

    float getLifeRatio();
    bool isGameOver();
    void damage(float amount);
};

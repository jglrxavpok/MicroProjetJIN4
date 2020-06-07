//
// Created by jglrxavpok on 02/06/2020.
//

#pragma once

class GameState {
private:
    float playerLife = 0.0f;
    float maxPlayerLife = 0.0f;
    bool won = false;

public:
    explicit GameState(float maxPlayerLife);

    float getLifeRatio();
    bool isGameOver();
    void damage(float amount);
    void win();
    bool isWon();

};

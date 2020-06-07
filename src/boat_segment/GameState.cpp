//
// Created by jglrxavpok on 02/06/2020.
//

#include "GameState.h"

GameState::GameState(float maxPlayerLife): maxPlayerLife(maxPlayerLife), playerLife(maxPlayerLife) {

}

float GameState::getLifeRatio() {
    return playerLife/maxPlayerLife;
}

bool GameState::isGameOver() {
    return playerLife <= 0.f;
}

void GameState::damage(float amount) {
    playerLife -= amount;
}

bool GameState::isWon() {
    return won;
}

void GameState::win() {
    won = true;
}

#include "Sound.h"
#include "BoatSegment.h"

Sound::Sound(std::string note) {
    buffer = std::move(Game::loadBuffer("resources/" + note + ".wav"));
    sound = Game::loadSound(buffer);

    successKeyImage = Game::loadTexture("resources/success"+note+".png");
    wrongKeyImage = Game::loadTexture("resources/wrong"+note+".png");
    pressKeyImage = Game::loadTexture("resources/press"+note+".png");
}

void Sound::play() {
    sound->play();
    blink = 0;
}

void Sound::render(sf::RenderWindow& renderTarget, float partialTick, sf::Sprite& keySprite, std::shared_ptr<sf::Texture> noKeyImage) {
    blink++;
    if (blink % 400 > 200) {
        keySprite.setTexture(*noKeyImage);
    }
    else {
        keySprite.setTexture(*pressKeyImage);
    }
}
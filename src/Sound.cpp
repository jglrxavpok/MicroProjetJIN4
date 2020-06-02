#include "Sound.h"
#include "Game.h"

Sound::Sound(std::string note) {
    buffer = std::move(loadBuffer("resources/" + note + ".wav"));
    sound = loadSound(buffer);

    successKeyImage = loadImage("resources/success"+note+".png");
    wrongKeyImage = loadImage("resources/wrong"+note+".png");
    pressKeyImage = loadImage("resources/press"+note+".png");
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
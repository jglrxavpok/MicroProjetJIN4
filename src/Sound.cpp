#include "Sound.h"
#include "boat_segment/BoatSegment.h"

Sound::Sound(std::string note) {
    buffer = std::move(Game::loadBuffer("resources/" + note + ".wav"));
    sound = Game::loadSound(buffer);

    successKeyImage = Game::loadTexture("resources/success"+note+".png");
    wrongKeyImage = Game::loadTexture("resources/wrong"+note+".png");
    pressKeyImage = Game::loadTexture("resources/press"+note+".png");
    singleImage = Game::loadTexture("resources/textures/note_"+note+".png");

    state = keyState::nothing;
}

void Sound::play() {
    sound->play();
    blink = 0;
}

void Sound::render(sf::RenderWindow& renderTarget, float partialTick, sf::Sprite& keySprite, std::shared_ptr<sf::Texture> noKeyImage) {
    blink++;
    switch (this->state) {
        case keyState::blinking:
            if (blink % 400 > 200) {
                keySprite.setTexture(*noKeyImage);
            }
            else {
                keySprite.setTexture(*pressKeyImage);
            }
            break;

        case keyState::right:
            keySprite.setTexture(*successKeyImage);
            break;

        case keyState::wrong:
            keySprite.setTexture(*wrongKeyImage);
            break;

        default:
            keySprite.setTexture(*noKeyImage);
            break;
    }
}

void Sound::setState(keyState state) {
    this->state = state;
}

keyState Sound::getState() {
    return state;
}

void Sound::renderSingle(sf::RenderTarget &renderTarget, float x, float y) {
    sf::Sprite sprite{*singleImage};
    sprite.setPosition(x, y);
    renderTarget.draw(sprite);
}

//
// Created by jglrxavpok on 07/06/2020.
//
#pragma once
#include <utility>

#include "GameplaySegment.h"
#include "Game.h"
#include "boat_segment/elements/FixedBackground.h"

template<class NextSegment>
class TransitionScreen: public GameplaySegment {

    bool fontLoaded = false;
    sf::Font defaultFont;
    bool noShader = false;
    bool shaderLoaded = false;
    sf::Shader oldMovieShader;
private:
    std::vector<std::wstring> lines;
    std::vector<sf::Text> linesToRender;
    sf::RenderTexture renderTexture;

    void renderContents(sf::RenderTarget& target, float partialTick);

public:
    explicit TransitionScreen(Game& game, std::vector<std::wstring> lines, int characterSize = 50);

    void render(sf::RenderWindow &renderTarget, float partialTick) override;

    void keyReleased(sf::Event::KeyEvent event) override;

    void update() override {
        time += Game::TARGET_UPDATE_PERIOD;
    }

    void shutdown() override {}
};



template<class NextSegment>
TransitionScreen<NextSegment>::TransitionScreen(Game &game, std::vector<std::wstring> lines, int characterSize): GameplaySegment(game), lines(std::move(lines)) {
    if(!fontLoaded) {
        if(!defaultFont.loadFromFile("resources/fonts/segoeprb.ttf")) {
            cerr << "Failed to load font!" << endl;
        }
        fontLoaded = true;
    }
    if(!shaderLoaded && !noShader) {
        if(!sf::Shader::isAvailable()) {
            cerr << "Shaders non disponibles sur cette machine. Tant pis." << endl;
            noShader = true;
        } else {
            if(!oldMovieShader.loadFromFile("resources/shaders/old_movie.frag", sf::Shader::Fragment)) {
                cerr << "Echec du chargement du shader, tant pis." << endl;
                noShader = true;
            }
            shaderLoaded = true;
        }
    }
    if(!renderTexture.create(1600,900)) {
        cerr << "Impossible de créer la renderTexture pour les transitions. On passe en mode sans shader." << endl;
        noShader = true;
    }
    for(const auto& text : this->lines) {
        linesToRender.emplace_back(text, defaultFont, characterSize);
    }

    auto background = make_unique<FixedBackground>(Game::loadTexture("resources/textures/transition_background.png"));
    scene = make_unique<Scene>(move(background));

    // alignement et placement des lignes
    float height = 0.0f;
    float spacing = 5.0f;
    for(const auto& line : linesToRender) {
        height += line.getLocalBounds().height + spacing;
    }

    float startY = 450.0f;
    for (int i = 0; i < linesToRender.size(); ++i) {
        auto& line = linesToRender[i];
        auto bounds = line.getLocalBounds();
        float x = 800.0f - bounds.width/2.0f;
        float offset = (float)i/linesToRender.size() - 0.5f;
        float y = offset*height + startY;
        line.setPosition(x, y);
    }
}

template<class NextSegment>
void TransitionScreen<NextSegment>::render(sf::RenderWindow &renderTarget, float partialTick) {
    if(noShader) {
        renderContents(renderTarget, partialTick);
    } else {
        renderTexture.clear();
        renderContents(renderTexture, partialTick);
        renderTexture.display();

        const sf::Texture& texture = renderTexture.getTexture();

        oldMovieShader.setUniform("iChannel0", sf::Shader::CurrentTexture);
        oldMovieShader.setUniform("iTime", time);

        sf::Sprite sprite(texture);
        renderTarget.draw(sprite, &oldMovieShader);
    }
}

template<class NextSegment>
void TransitionScreen<NextSegment>::keyReleased(sf::Event::KeyEvent event) {
    if(event.code == sf::Keyboard::Space) {
        game.setGameplay(move(make_unique<NextSegment>(game)));
    }
}

template<class NextSegment>
void TransitionScreen<NextSegment>::renderContents(sf::RenderTarget &target, float partialTick) {
    if(scene) {
        scene->renderAll(target, partialTick);


        for(const auto& line : linesToRender) {
            target.draw(line);
        }
    }
}

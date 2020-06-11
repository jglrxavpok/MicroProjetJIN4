//
// Created by jglrxavpok on 28/05/2020.
//

#include "elements/LoopingBackground.h"
#include "elements/BoatElement.h"
#include "elements/MusicLineElement.h"
#include "elements/FinishLineElement.h"
#include "elements/DecorationElement.h"
#include "elements/CollisionElement.h"
#include "BoatSegment.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/FreeFuncs.hpp>
#include "specialscreens/TransitionScreen.hpp"
#include "specialscreens/GameOverScreen.hpp"
#include "specialscreens/VictoryScreen.h"

BoatSegment::BoatSegment(Game& game): GameplaySegment(game) {
    badGuyTexture = Game::loadTexture("resources/textures/bad_guy.png");
    singleNotesTexture = Game::loadTexture("resources/textures/single_notes_spritesheet.png");
    doubleNotesTexture = Game::loadTexture("resources/textures/double_notes_spritesheet.png");
    finishLineUITexture = Game::loadTexture("resources/textures/ui_finish_line.png");
    boatProgressUITexture = Game::loadTexture("resources/textures/ui_boat.png");

    finishLineSprite.setTexture(*finishLineUITexture);
    boatProgressSprite.setTexture(*boatProgressUITexture);

    unique_ptr<SceneElement> background = make_unique<LoopingBackground>(Game::loadTexture("resources/textures/boat_background.png"));
    scene = make_unique<Scene>(move(background));

    hurtSoundBuffer = Game::loadBuffer("resources/sounds/hurt.wav");
    lineBreakSoundBuffer = Game::loadBuffer("resources/sounds/line_break.wav");
    lineKillsSoundBuffer = Game::loadBuffer("resources/sounds/line_cycle.wav");

    scene->addElement(make_unique<BoatElement>(state, Game::loadSound(hurtSoundBuffer)));

    tmx::Map map;
    map.load("resources/maps/styx.tmx");

    b2BodyDef levelCollisionsDef;
    levelCollisionsDef.type = b2_staticBody;
    levelCollisions = scene->getPhysicsWorld().CreateBody(&levelCollisionsDef);

    for(const auto& layer : map.getLayers()) {
        loadLayer(*layer);
    }
}

void BoatSegment::loadLayer(tmx::Layer &layer) {
    if(layer.getType() == tmx::Layer::Type::Object) {
        auto& objects = layer.getLayerAs<tmx::ObjectGroup>();
        for(auto& object : objects.getObjects()) {
            const string& type = object.getType();
            if(object.getTileID() != 0) { // décoration
                // TODO: sélection des images
                auto texture = autoloadTexture("resources/textures/tmp.png");
                scene->addElement(make_unique<DecorationElement>(texture, object.getAABB().left, object.getAABB().top-object.getAABB().height, object.getAABB().width, object.getAABB().height));
            } else if(type == "collision") { // collision du niveau
                loadCollision(object);
            } else if(type == "enemy") { // point de spawn d'un ennemi
                auto enemy = make_unique<EnemyElement>(renderTarget, badGuyTexture);
                enemy->getPosition().x = object.getPosition().x;
                enemy->getPosition().y = object.getPosition().y;
                scene->addElement(move(enemy));
            } else if(type == "finishLine") { // ligne d'arrivée
                finishLineX = object.getPosition().x;
                scene->addElement(make_unique<FinishLineElement>(object, state));
            } else { // on sait pas, on ajoute la collision au cas où
                scene->addElement(make_unique<CollisionElement>(object));
            }
        }
    } else if(layer.getType() == tmx::Layer::Type::Group) {
        auto& group = layer.getLayerAs<tmx::LayerGroup>();
        for(const auto& l : group.getLayers()) {
            loadLayer(*l);
        }
    } else if(layer.getType() == tmx::Layer::Type::Image) {
        auto& image = layer.getLayerAs<tmx::ImageLayer>();
        std::string texturePath = tmx::resolveFilePath(image.getImagePath(), "");
        auto texture = autoloadTexture(texturePath);
        scene->addElement(make_unique<DecorationElement>(texture, image.getOffset().x, image.getOffset().y));
    }
}

void BoatSegment::loadCollision(const tmx::Object& obj) {
    // ajout de la forme de collision au body 'levelCollisions'
    createFixture(levelCollisions, obj);
}

b2Fixture * BoatSegment::createFixture(b2Body *body, const tmx::Object &obj) {
    b2PolygonShape shape;
    vector<b2Vec2> shapePolygon;
    switch(obj.getShape()) {
        case tmx::Object::Shape::Rectangle: {
            auto& aabb = obj.getAABB();
            shapePolygon.emplace_back(aabb.left, aabb.top);
            shapePolygon.emplace_back(aabb.left+aabb.width, aabb.top);
            shapePolygon.emplace_back(aabb.left+aabb.width, aabb.top+aabb.height);
            shapePolygon.emplace_back(aabb.left, aabb.top+aabb.height);
            break;
        }

        case tmx::Object::Shape::Polygon: {
            for(const auto& p : obj.getPoints()) {
                shapePolygon.emplace_back(p.x, p.y);
            }
            break;
        }

        default:
            cerr << "Unknown shape for tmx::Object used as collision: " << (int)obj.getShape() << endl;
            assert(false); // unknown shape
    }
    shape.Set(shapePolygon.data(), shapePolygon.size());

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;
    return body->CreateFixture(&fixture);
}

void BoatSegment::update() {
    if(state.isGameOver()) {
        game.setGameplay(make_unique<GameOverScreen<BoatSegment>>(game));
        return;
    }
    if(state.isWon()) {
        vector<wstring> text = {L"C'est la fin du jeu.", L"On espère que ça vous a plu.", L"", L"L'écran de fin est juste après."};
        game.setGameplay(move(make_unique<TransitionScreen<VictoryScreen>>(game, text)));
        return;
    }

    if(currentMusicLine) {
        if(currentMusicLine->isBroken()) {
            currentMusicLine = nullptr;
        }
    }
    if(scene) {
        scene->updateAll(Game::TARGET_UPDATE_PERIOD);

        time += Game::TARGET_UPDATE_PERIOD;
    }
}

void BoatSegment::renderProgressBar() {
    auto& view = renderTarget.getView();

    sf::RectangleShape background(sf::Vector2f(view.getSize().x-40.0f, 50.0f));
    background.setPosition(view.getSize().x/2.0f - background.getSize().x/2.0f, view.getSize().y-background.getSize().y-20.0f);
    background.setFillColor(sf::Color(38,38,38,255));

    finishLineSprite.setPosition(background.getPosition().x+background.getSize().x-finishLineUITexture->getSize().x, background.getPosition().y);

    float progress = scene->getPlayer().getPosition().x / finishLineX;
    float playerX = progress * background.getSize().x + background.getPosition().x;
    // centré verticalement sur background
    float playerY = background.getPosition().y + background.getSize().y / 2.0f - boatProgressUITexture->getSize().y / 2.0f;
    boatProgressSprite.setPosition(playerX, playerY);

    renderTarget.draw(background);
    renderTarget.draw(finishLineSprite);
    renderTarget.draw(boatProgressSprite);
}

void BoatSegment::renderHealthBar() {
    auto& view = renderTarget.getView();

    sf::RectangleShape background(sf::Vector2f(view.getSize().x-40.0f, 50.0f));
    background.setPosition(view.getSize().x/2.0f - background.getSize().x/2.0f, 20.0f);
    background.setFillColor(sf::Color(38,38,38,255));


    float yPercent = 0.8f;
    float xPercent = 0.95f;
    sf::RectangleShape lifeBar(sf::Vector2f(background.getSize().x*xPercent*state.getLifeRatio(), background.getSize().y*yPercent));
    lifeBar.setPosition(view.getSize().x/2.0f - background.getSize().x*xPercent/2.0f, background.getPosition().y+background.getSize().y*(1.0f-yPercent)/2.0f);
    lifeBar.setFillColor(sf::Color(38,198,38,255));

    renderTarget.draw(background);
    renderTarget.draw(lifeBar);
}

void BoatSegment::render(sf::RenderWindow& renderTarget, float partialTick) {
    renderTarget.clear(sf::Color::White);

    if(scene) {
        scene->renderAll(renderTarget, partialTick);

        renderTarget.setView(scene->getRenderView());
        if(currentMusicLine) {
            currentMusicLine->debugRender(renderTarget);
        }
        renderTarget.setView(renderTarget.getDefaultView());
    }

    // render UI
    renderHealthBar();
    renderProgressBar();
}

float lastX = 0;
float lastY = 0;

void BoatSegment::mousePressed(int x, int y, sf::Mouse::Button button) {

    auto coords = renderTarget.mapPixelToCoords(sf::Vector2i(x, y), scene->getRenderView());
    lastX = coords.x;
    lastY = coords.y;
    currentMusicLine = make_shared<MusicLine>(scene, singleNotesTexture, doubleNotesTexture, Game::loadSound(lineBreakSoundBuffer), Game::loadSound(lineKillsSoundBuffer));
}

void BoatSegment::mouseReleased(int x, int y, sf::Mouse::Button button) {
    if(currentMusicLine) {
        currentMusicLine->destroySurroundedEnemies();
    }
    currentMusicLine = nullptr;
}

void BoatSegment::mouseDrag(int x, int y, int dx, int dy, sf::Mouse::Button button) {
    if(!currentMusicLine) {
        return;
    }
    if(button == sf::Mouse::Left) {
        auto endCoords = renderTarget.mapPixelToCoords(sf::Vector2i(x, y), scene->getRenderView());
        float deltaX = endCoords.x - lastX;
        float deltaY = endCoords.y - lastY;
        if(deltaX*deltaX+deltaY*deltaY >= 30*30) { // plus de 20 pixels de distance
            currentMusicLine->addLine(lastX, lastY, endCoords.x, endCoords.y);

            lastX = endCoords.x;
            lastY = endCoords.y;
        }
    }
}

void BoatSegment::shutdown() {

}

BoatSegment::~BoatSegment() {
    scene->getPhysicsWorld().DestroyBody(levelCollisions);
}

shared_ptr<sf::Texture> BoatSegment::autoloadTexture(std::string texturePath) {
    auto it = autoloadedTextures.find(texturePath);

    if(it == autoloadedTextures.end()) { // pas encore chargé
        autoloadedTextures[texturePath] = Game::loadTexture(texturePath);
    }

    return autoloadedTextures[texturePath];
}

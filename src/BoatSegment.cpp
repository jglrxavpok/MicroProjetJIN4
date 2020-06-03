//
// Created by jglrxavpok on 28/05/2020.
//

#include "elements/LoopingBackground.h"
#include "elements/BoatElement.h"
#include "elements/ShoreElement.h"
#include "elements/PlayerLineElement.h"
#include "BoatSegment.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/ImageLayer.hpp>

BoatSegment::BoatSegment(Game& game): GameplaySegment(game) {
    badGuyTexture = Game::loadTexture("resources/textures/bad_guy.png");
    singleNotesTexture = Game::loadTexture("resources/textures/single_notes_spritesheet.png");
    doubleNotesTexture = Game::loadTexture("resources/textures/double_notes_spritesheet.png");

    unique_ptr<SceneElement> background = make_unique<LoopingBackground>(Game::loadTexture("resources/textures/boat_background.png"));
    scene = make_unique<Scene>(move(background));

    scene->addElement(make_unique<BoatElement>(state));

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
            if(type == "collision") {
                loadCollision(object);
            } else if(type == "enemy") {
                auto enemy = make_unique<EnemyElement>(renderTarget, badGuyTexture);
                enemy->getPosition().x = object.getPosition().x;
                enemy->getPosition().y = object.getPosition().y;
                scene->addElement(move(enemy));
            }
            // TODO: obstacles, finish line, etc.
        }
    } else if(layer.getType() == tmx::Layer::Type::Group) {
        auto& group = layer.getLayerAs<tmx::LayerGroup>();
        for(const auto& l : group.getLayers()) {
            loadLayer(*l);
        }
    } else if(layer.getType() == tmx::Layer::Type::Image) {
        // TODO
    }
}

void BoatSegment::loadCollision(const tmx::Object& obj) {
    // ajout de la forme de collision au body 'levelCollisions'

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

        default:
            cerr << "Unknown shape for tmx::Object used as collision: " << (int)obj.getShape() << endl;
            assert(false); // unknown shape
    }
    shape.Set(shapePolygon.data(), shapePolygon.size());

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;
    levelCollisions->CreateFixture(&fixture);
}

void BoatSegment::spawnEnemy() {
    auto enemy = make_unique<EnemyElement>(renderTarget, badGuyTexture);
    float centerY = rng.rand(ShoreElement::HEIGHT, 822.5f);
    auto coords = renderTarget.mapPixelToCoords(sf::Vector2i(1600-200, (int)centerY), scene->getRenderView());

    enemy->getPosition() = coords;
    scene->addElement(move(enemy));
}

void BoatSegment::update() {
    if(state.isGameOver()) {
        // TODO: switch to game over screen :c
    }
    if(scene) {
        scene->updateAll(Game::TARGET_UPDATE_PERIOD);

        time += Game::TARGET_UPDATE_PERIOD;

        if(time >= ENEMY_SPAWN_PERIOD) {
        //    spawnEnemy();
            time -= ENEMY_SPAWN_PERIOD;
        }
    }
}

void BoatSegment::renderHealthBar() {
    auto& view = renderTarget.getView();

    sf::RectangleShape background(sf::Vector2f(view.getSize().x-40.0f, 50.0f));
    background.setPosition(view.getSize().x/2.0f - background.getSize().x/2.0f, view.getSize().y-background.getSize().y-20.0f);
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
}

float lastX = 0;
float lastY = 0;

void BoatSegment::mousePressed(int x, int y, sf::Mouse::Button button) {
    auto coords = renderTarget.mapPixelToCoords(sf::Vector2i(x, y), scene->getRenderView());
    lastX = coords.x;
    lastY = coords.y;
    currentMusicLine = make_shared<MusicLine>(scene, singleNotesTexture, doubleNotesTexture);
}

void BoatSegment::mouseReleased(int x, int y, sf::Mouse::Button button) {
    if(currentMusicLine) {
        currentMusicLine->destroySurroundedEnemies();
    }
    currentMusicLine = nullptr;
}

void BoatSegment::mouseDrag(int x, int y, int dx, int dy, sf::Mouse::Button button) {
    if(button == sf::Mouse::Left) {
        auto endCoords = renderTarget.mapPixelToCoords(sf::Vector2i(x, y), scene->getRenderView());
        float deltaX = endCoords.x - lastX;
        float deltaY = endCoords.y - lastY;
        if(deltaX*deltaX+deltaY*deltaY >= 30*30) { // plus de 20 pixels de distance
            if(currentMusicLine) {
                currentMusicLine->addLine(lastX, lastY, endCoords.x, endCoords.y);
            }

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

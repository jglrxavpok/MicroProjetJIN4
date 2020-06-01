//
// Created by jglrxavpok on 30/05/2020.
//

#include <elements/PlayerLineElement.h>
#include "MusicLine.h"

MusicLine::MusicLine(std::unique_ptr<Scene>& scene): scene(scene) {

}

void MusicLine::addLine(float startX, float startY, float endX, float endY) {
    auto partReference = make_shared<MusicLinePart>(startX, startY, endX, endY);
    parts.emplace_back(partReference);
    scene->addElement(make_unique<PlayerLineElement>(shared_from_this(), partReference, startX, startY, endX, endY));
    updateGraph();
}

bool MusicLine::surrounds(unique_ptr<SceneElement> &) {
    // TODO: 2. utiliser les polygônes sauvegardés pour trouver si l'élément (du moins son centre)
    return false;
}

void MusicLine::destroyEnemy(EnemyElement* enemy) {
    // TODO: add feedback to player
    enemy->scheduleForRemoval();
}

void MusicLine::destroySurroundedEnemies() {
    updateGraph(); // les morceaux ont pu expirer depuis la dernière MàJ
    for(auto& elem : scene->getElements()) {
        auto* nakedPtr = elem.get();
        if(auto* enemy = dynamic_cast<EnemyElement*>(nakedPtr)) {
            if(surrounds(elem)) {
                destroyEnemy(enemy);
            }
        }
    }
}

void MusicLine::updateGraph() {
    parts.erase(remove_if(parts.begin(), parts.end(), [](const auto w) { return w.expired(); }), parts.end());

    // TODO: 1. créer des polygônes correspondants aux boucles créées par l'ajout des éléments
    // cf https://stackoverflow.com/questions/35456877/how-to-detect-all-regions-that-are-surrounded-by-n-line-segments ?
    // https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm
}

int MusicLine::countParts() {
    updateGraph();
    return parts.size();
}

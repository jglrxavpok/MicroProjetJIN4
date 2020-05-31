//
// Created by jglrxavpok on 30/05/2020.
//

#include <elements/PlayerLineElement.h>
#include "MusicLine.h"

MusicLine::MusicLine(std::unique_ptr<Scene>& scene): scene(scene) {

}

void MusicLine::addLine(float startX, float startY, float endX, float endY) {
    scene->addElement(make_unique<PlayerLineElement>(shared_from_this(), startX, startY, endX, endY));
    // TODO: 0. Comment gérer les éléments qui ont expiré?
    // TODO: 1. créer des polygônes correspondants aux boucles créées par l'ajout des éléments
    // cf https://stackoverflow.com/questions/35456877/how-to-detect-all-regions-that-are-surrounded-by-n-line-segments ?
    // https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm
    // TODO: Briser les lignes qui rentrent en contact avec un ennemi? (à la Pokémon Ranger)
}

bool MusicLine::surrounds(unique_ptr<SceneElement> &) {
    // TODO: 2. utiliser les polygônes sauvegardés pour trouver si l'élément (du moins son centre)
    return false;
}

void MusicLine::destroyEnemy(EnemyElement* enemy) {
    // TODO
    enemy->scheduleForRemoval();
}

void MusicLine::destroySurroundedEnemies() {
    for(auto& elem : scene->getElements()) {
        auto* nakedPtr = elem.get();
        if(auto* enemy = dynamic_cast<EnemyElement*>(nakedPtr)) {
            if(surrounds(elem)) {
                destroyEnemy(enemy);
            }
        }
    }
}

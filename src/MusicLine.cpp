//
// Created by jglrxavpok on 30/05/2020.
//

#include <elements/PlayerLineElement.h>
#include "MusicLine.h"

MusicLine::MusicLine(std::unique_ptr<Scene>& scene): scene(scene) {

}

void MusicLine::addLine(float startX, float startY, float endX, float endY) {
    scene->addElement(make_unique<PlayerLineElement>(shared_from_this(), startX, startY, endX, endY));
}

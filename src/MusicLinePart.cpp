//
// Created by jglrxavpok on 01/06/2020.
//

#include "MusicLinePart.h"
#include "math_help.h"

MusicLinePart::MusicLinePart(float startX, float startY, float endX, float endY): startX(startX), startY(startY), endX(endX), endY(endY) {

}

bool MusicLinePart::intersects(MusicLinePart &other, sf::Vector2f& intersectionOut) {
    return intersectionTest(startX, startY, endX, endY, other.startX, other.startY, other.endX, other.endY, intersectionOut);
}

bool MusicLinePart::isBroken() {
    return shouldBreak;
}

void MusicLinePart::breakLine() {
    shouldBreak = true;
}

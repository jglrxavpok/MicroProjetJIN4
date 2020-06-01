//
// Created by jglrxavpok on 01/06/2020.
//

#include "MusicLinePart.h"

MusicLinePart::MusicLinePart(float startX, float startY, float endX, float endY): startX(startX), startY(startY), endX(endX), endY(endY) {

}

bool MusicLinePart::intersects(MusicLinePart &other, sf::Vector2f& intersectionOut) {
    float determinant = (startX-endX) * (other.startY - other.endY) - (startY - endY) * (other.startX - other.endX);
    if(determinant < 0.001f && determinant > -0.001f) {
        return false;
    }
    float tDeterminant = (startX - other.startX) * (other.startY - other.endY) - (startY - other.startY) * (other.startX - other.endX);
    float uDeterminant = - ((startX - endX) * (startY - other.startY) - (startY - endY) * (startX - other.startX));
    float divisor = (startX-endX) * (other.startY-other.endY) - (startY-endY) * (other.startX-other.endX);
    float t = tDeterminant / divisor;
    float u = uDeterminant / divisor;
    if(t > 0.0f && t < 1.0f && u > 0.0f && u < 1.0f) {
        intersectionOut.x = t*endX + (1.0f-t)*startX;
        intersectionOut.y = t*endY + (1.0f-t)*startY;
        return true;
    }
    return false;
}

//
// Created by jglrxavpok on 02/06/2020.
//

#pragma once
#include <SFML/Graphics.hpp>

/// Teste l'intersection entre deux segments définis par leur extrémités
/// \param startX
/// \param startY
/// \param endX
/// \param endY
/// \param otherStartX
/// \param otherStartY
/// \param otherEndX
/// \param otherEndY
/// \param intersectionOut le vecteur dans lequel stocker l'intersection, s'il y a en une
/// \return true ssi il y a une intersection
bool intersectionTest(float startX, float startY, float endX, float endY, float otherStartX, float otherStartY, float otherEndX, float otherEndY, sf::Vector2f& intersectionOut);

sf::Color randomColor();
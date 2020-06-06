//
// Created by jglrxavpok on 05/06/2020.
//

#pragma once
#include "SceneElement.h"
#include "CollisionElement.h"
#include <tmxlite/Object.hpp>
#include <box2d/box2d.h>

class FinishLineElement: public CollisionElement {
public:
    explicit FinishLineElement(const tmx::Object& source);

    void beginContact(SceneElement *other) override;
};


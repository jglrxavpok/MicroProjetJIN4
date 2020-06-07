//
// Created by jglrxavpok on 05/06/2020.
//

#pragma once
#include "SceneElement.h"
#include "CollisionElement.h"
#include <tmxlite/Object.hpp>
#include <box2d/box2d.h>
#include "boat_segment/GameState.h"

class FinishLineElement: public CollisionElement {
private:
    GameState& state;

public:
    explicit FinishLineElement(const tmx::Object& source, GameState& state);

    void beginContact(SceneElement *other) override;
};


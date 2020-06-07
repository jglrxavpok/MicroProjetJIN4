//
// Created by jglrxavpok on 05/06/2020.
//

#include "FinishLineElement.h"
#include "BoatElement.h"
#include "boat_segment/BoatSegment.h"
#include <iostream>

FinishLineElement::FinishLineElement(const tmx::Object &source, GameState& state): CollisionElement(source), state(state) {}

void FinishLineElement::beginContact(SceneElement *other) {
    if(auto* player = dynamic_cast<BoatElement*>(other)) {
        state.win();
    }
}

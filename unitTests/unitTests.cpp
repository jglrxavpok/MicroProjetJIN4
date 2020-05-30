#include <gtest/gtest.h>
#include <elements/PlayerLineElement.h>
#include "Scene.h"
#include "MusicLine.h"

namespace myNameSpace {

    TEST(TestMusicLine, TestDestructionWhenChildrenAreDead) {
        auto scene = make_unique<Scene>(nullptr);
        auto line = make_shared<MusicLine>(scene);

        // on ajoute 10 éléments
        for (int i = 0; i < 10; ++i) {
            line->addLine(0,0,1,1);
        }
        auto weakLine = line->weak_from_this();
        ASSERT_FALSE(weakLine.expired()); // le pointeur doit être valide à cet instant

        // on supprime la référence à MusicLine
        line = nullptr;

        ASSERT_FALSE(weakLine.expired()); // le pointeur doit rester valide à cet instant, les enfants ont encore la référence

        // on avance dans le temps, les enfants doivent toujours être vivants
        scene->updateAll(PlayerLineElement::MAX_LIFETIME/2.0f);
        ASSERT_FALSE(weakLine.expired()); // le pointeur doit rester valide à cet instant, les enfants ont toujours et encore la référence

        // on fait mourir les enfants
        scene->updateAll(PlayerLineElement::MAX_LIFETIME/2.0f);

        // le weak pointeur doit maintenant être invalide
        ASSERT_TRUE(weakLine.expired());
    }
}

#include <gtest/gtest.h>
#include <elements/PlayerLineElement.h>
#include <elements/EnemyElement.h>
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

    TEST(TestMusicLine, TestSurroundEnemies) {
        auto scene = make_unique<Scene>(nullptr);
        auto line = make_shared<MusicLine>(scene);

        int steps = 6;
        float ang = 0.0f;
        float step = 2.0f*M_PI/steps;
        float radius = 1.0f;

        int lineCount = steps+1; // +1 pour s'assurer d'avoir une intersection
        for (int i = 0; i < steps; ++i) {
            float preX = cos(ang) * radius;
            float preY = sin(ang) * radius;
            ang += step;
            float x = cos(ang) * radius;
            float y = sin(ang) * radius;
            line->addLine(preX, preY, x, y);
        }

        ASSERT_EQ(0, line->countCycles());
        float x = cos(ang) * radius;
        float y = sin(ang) * radius;
        line->addLine(x, y, -0.5f, -0.5f);
        ASSERT_EQ(0, line->countCycles());
        line->addLine(-0.5f, -0.5f, 0.0f, 10.0f);// on force une intersection

        ASSERT_EQ(1, line->countCycles());

        unique_ptr<SceneElement> enemy1 = make_unique<EnemyElement>(nullptr);
        unique_ptr<SceneElement> enemy2 = make_unique<EnemyElement>(nullptr);
        enemy2->getPosition() = sf::Vector2f(100.0f, 100.0f); // outside of circle

        ASSERT_TRUE(line->surrounds(enemy1));
        ASSERT_FALSE(line->surrounds(enemy2));

        scene->addElement(move(enemy1));
        scene->addElement(move(enemy2));
        ASSERT_EQ(lineCount + 2, scene->getElements().size());
        line->destroySurroundedEnemies();
        ASSERT_EQ(lineCount + 1, scene->getElements().size());
    }

    TEST(TestMusicLine, PartReferenceExpiration) {
        auto scene = make_unique<Scene>(nullptr);
        auto line = make_shared<MusicLine>(scene);

        line->addLine(0,0,0,0);
        ASSERT_EQ(1, line->countParts());
        scene->updateAll(PlayerLineElement::MAX_LIFETIME/2.0f);
        ASSERT_EQ(1, line->countParts());
        scene->updateAll(PlayerLineElement::MAX_LIFETIME/2.0f);
        ASSERT_EQ(0, line->countParts());
    }
}

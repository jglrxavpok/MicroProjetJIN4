#include <gtest/gtest.h>
#include <boat_segment/elements/MusicLineElement.h>
#include <boat_segment/elements/EnemyElement.h>
#include "Scene.h"
#include "boat_segment/MusicLine.h"

#include "MusicSegment.h"

#define M_PI 3.1415926535

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
        scene->updateAll(MusicLineElement::MAX_LIFETIME / 2.0f);
        ASSERT_FALSE(weakLine.expired()); // le pointeur doit rester valide à cet instant, les enfants ont toujours et encore la référence

        // on fait mourir les enfants
        scene->updateAll(MusicLineElement::MAX_LIFETIME / 2.0f);

        // le weak pointeur doit maintenant être invalide
        ASSERT_TRUE(weakLine.expired());
    }

    TEST(TestMusicLine, TestSurroundEnemies) {
        auto scene = make_unique<Scene>(nullptr);
        auto line = make_shared<MusicLine>(scene);
        scene->disableContactPropagationForTesting();

        int steps = 6;
        float ang = 0.0f;
        float step = 2.0f*M_PI/steps;
        float radius = 1.0f;

        int lineCount = steps+2; // +2 pour s'assurer d'avoir une intersection
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
        line->addLine(x, y, 0.5f, 0.5f);
        ASSERT_EQ(0, line->countCycles());
        line->addLine(0.5f, 0.5f, 0.0f, 10.0f);// on force une intersection

        ASSERT_EQ(1, line->countCycles());

        sf::RenderWindow testWindow;
        unique_ptr<SceneElement> enemy1 = make_unique<EnemyElement>(testWindow, nullptr);
        unique_ptr<SceneElement> enemy2 = make_unique<EnemyElement>(testWindow, nullptr);
        enemy2->getPosition() = sf::Vector2f(100.0f, 100.0f); // outside of circle

        ASSERT_TRUE(line->surrounds(enemy1));
        ASSERT_FALSE(line->surrounds(enemy2));

        scene->addElement(move(enemy1));
        scene->addElement(move(enemy2));
        ASSERT_EQ(lineCount + 2, scene->getElements().size());
        line->destroySurroundedEnemies();
        scene->updateAll(0.0f); // permet de nettoyer les éléments
        ASSERT_EQ(lineCount + 1, scene->getElements().size());
    }

    TEST(TestMusicLine, PartReferenceExpiration) {
        auto scene = make_unique<Scene>(nullptr);
        auto line = make_shared<MusicLine>(scene);

        line->addLine(0,0,0,0);
        ASSERT_EQ(1, line->countParts());
        scene->updateAll(MusicLineElement::MAX_LIFETIME / 2.0f);
        ASSERT_EQ(1, line->countParts());
        scene->updateAll(MusicLineElement::MAX_LIFETIME / 2.0f);
        ASSERT_EQ(0, line->countParts());
    }

    TEST(TestLoosingLife, NotPlaying) {
        /*sf::RenderWindow window;
        Game game(window);
        auto musicSegment = make_unique<MusicSegment>(game);
        for (int k = 0; k < TEMPO; k++) {
            musicSegment->update();
        }
        ASSERT_EQ(NBLIFE - 1, musicSegment->getLives());*/
    }
}

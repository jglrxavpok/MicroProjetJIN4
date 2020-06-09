//
// Created by jglrxavpok on 30/05/2020.
//

#pragma once
#include <memory>
#include <iostream>
#include "Scene.h"
#include "elements/EnemyElement.h"
#include "MusicLinePart.h"
#include "RNG.h"
#include <SFML/Audio.hpp>

class GraphCycle {
private:
    // AABB pour des tests rapides
    float minX;
    float maxX;
    float minY;
    float maxY;

public:
    vector<sf::Vector2f> vertices;
    explicit GraphCycle(vector<sf::Vector2f> vertices);

    bool isInside(const sf::Vector2f& point);
};

/// Permet de gérer le comportement d'une ligne de musique dessinée par le joueur avec la souris
/// Responsable de l'ajout des éléments à la scène, de vérifier si des ennemis sont à l'intérieur de cercles, etc.
///
/// Est détruite quand ces enfants PlayerLineElement sont tous détruits. Pour que cela fonctionne, DOIT être utilisé avec un shared_ptr
class MusicLine: public std::enable_shared_from_this<MusicLine> {
private:
    /// Scène dans laquelle ajouter les éléments
    unique_ptr<Scene>& scene;

    /// références aux morceaux de la ligne. Permet de connaître
    vector<weak_ptr<MusicLinePart>> parts;
    vector<unique_ptr<GraphCycle>> cycles;

    shared_ptr<sf::Texture> singleNotesSpritesheet;
    shared_ptr<sf::Texture> doubleNotesSpritesheet;
    shared_ptr<sf::Sound> breakSound;
    shared_ptr<sf::Sound> killSound;

    RNG rng{};

    // TODO: améliorer le code pour la recherche de cycle
    typedef struct {
        int index;
        float x;
        float y;
        bool partOfCycle;
        int value;
        shared_ptr<MusicLinePart> firstLine;
        shared_ptr<MusicLinePart> secondLine;
    } vertex;

    typedef map<int, vector<int>> adjacency_map;

    /// Permet de savoir si cette ligne a été brisée. Surtout utile lors du dessin de la ligne pour savoir si on doit commencer une nouvelle ligne
    bool broken = false;

    /// mets à jour le graphe permettant de trouver les ennemis entourés
    /// Prends aussi en compte les lignes qui ont pu expirer
    void updateGraph();

    void destroyEnemy(EnemyElement* enemy);

    /// Cherches un cycle dans un graphe défini par ses sommets et arêtes
    unique_ptr<GraphCycle> findCycle(vertex* start, vector<vertex>& allVertices, adjacency_map& adjacency);

    /// responsable de la reconstruction d'un cycle lorsqu'un cycle est détecté
    unique_ptr<GraphCycle> reconstructCycle(vertex *endPoint, vertex* secondParent, map<vertex *, vertex *>& parents);

    /// Connecte 'v' aux deux points les plus proches présents sur la ligne 'along'
    void connectToClosest2(vertex* v, vector<vertex>& allVertices, adjacency_map& adjacency, const shared_ptr<MusicLinePart>& along);

    /// Supprime de parts les weak_ptr qui ont expiré
    void updateParts();
public:
    explicit MusicLine(unique_ptr<Scene>& scene, shared_ptr<sf::Texture> singleNotesSpritesheet = nullptr, shared_ptr<sf::Texture> doubleNotesSpritesheet = nullptr, shared_ptr<sf::Sound> breakSound = nullptr, shared_ptr<sf::Sound> killSound = nullptr);
    void addLine(float startX, float startY, float endX, float endY);

    /// Renvoies 'true' ssi cette ligne de musique entoure l'élement donné (ie cette ligne crée une boucle et que
    /// l'élément est au centre de cette boucle)
    /// @param element l'élément à vérifier. N'a pas besoin d'être dans la scène
    bool surrounds(unique_ptr<SceneElement>& element);
    void destroySurroundedEnemies();

    int countParts();
    int countCycles();

    /// Casse chacun des morceaux de la ligne. Appelé lorsqu'un ennemi entre en contact avec un morceau
    void breakLine();

    /// cf broken
    bool isBroken();

    void debugRender(sf::RenderWindow& target);

    ~MusicLine() = default;
};

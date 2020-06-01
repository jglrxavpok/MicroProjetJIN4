//
// Created by jglrxavpok on 30/05/2020.
//

#include <elements/PlayerLineElement.h>
#include "MusicLine.h"
#include <queue>

MusicLine::MusicLine(std::unique_ptr<Scene>& scene): scene(scene) {

}

void MusicLine::addLine(float startX, float startY, float endX, float endY) {
    auto partReference = make_shared<MusicLinePart>(startX, startY, endX, endY);
    parts.emplace_back(partReference);
    scene->addElement(make_unique<PlayerLineElement>(shared_from_this(), partReference, startX, startY, endX, endY));
    updateGraph();
}

bool MusicLine::surrounds(unique_ptr<SceneElement> &) {
    // TODO: 2. utiliser les polygônes sauvegardés pour trouver si l'élément (du moins son centre)
    return false;
}

void MusicLine::destroyEnemy(EnemyElement* enemy) {
    // TODO: add feedback to player
    enemy->scheduleForRemoval();
}

void MusicLine::destroySurroundedEnemies() {
    updateGraph(); // les morceaux ont pu expirer depuis la dernière MàJ
    for(auto& elem : scene->getElements()) {
        auto* nakedPtr = elem.get();
        if(auto* enemy = dynamic_cast<EnemyElement*>(nakedPtr)) {
            if(surrounds(elem)) {
                destroyEnemy(enemy);
            }
        }
    }
}

void MusicLine::updateGraph() {
    cycles.clear();
    parts.erase(remove_if(parts.begin(), parts.end(), [](const auto w) { return w.expired(); }), parts.end());

    // TODO: 1. créer des polygônes correspondants aux boucles créées par l'ajout des éléments
    // cf https://stackoverflow.com/questions/35456877/how-to-detect-all-regions-that-are-surrounded-by-n-line-segments ?

    // Attention, l'algorithme ci-dessus est sub-optimal
    // on commence par chercher les intersections des lignes
    vector<vertex> vertices;


    for(int i = 0;i<parts.size();i++) {
        auto& wl1 = parts[i];
        auto l1 = wl1.lock();
        if(i < parts.size()-1) {
            auto& wl2 = parts[i+1];
            auto l2 = wl2.lock();
            vertices.push_back({.x = l1->endX, .y = l1->endY, .partOfCycle = false, .value = 0, .l1 = l1, .l2 = l2});
        } else {
            vertices.push_back({.x = l1->endX, .y = l1->endY, .partOfCycle = false, .value = 0, .l1 = l1, .l2 = nullptr});
        }

        for(int j = 0;j<parts.size();j++) {
            auto& wl2 = parts[j];
            auto l2 = wl2.lock();
            if(l1 != l2) {
                sf::Vector2f intersectionPoint(0,0);
                if(l1->intersects(*l2, intersectionPoint)) {
                    cout << "found intersection: " << intersectionPoint.x << ", " << intersectionPoint.y << endl;
                    vertices.push_back({.x = intersectionPoint.x, .y = intersectionPoint.y, .partOfCycle = false, .value = 0, .l1 = l1, .l2 = l2});
                }
            }
        }
    }

    // on crée les arêtes du graphe
    map<vertex*, vector<vertex*>> adjacency; // TODO: multimap
    for (int i = 0; i < vertices.size(); ++i) {
        for (int j = 0; j < vertices.size(); ++j) {
            if(i != j) {
                vertex& a = vertices[i];
                vertex& b = vertices[j];
                if(a.x == b.x && a.y == b.y) {
                    continue;
                }
                // si les deux points sur la même ligne
                if(a.l1 == b.l1 || a.l1 == b.l2 || a.l2 == b.l1 || a.l2 == b.l2) {
                    auto* aPtr = &vertices[i];
                    auto* bPtr = &vertices[j];
                    auto& aAdjacency = adjacency[aPtr];
                    auto& bAdjacency = adjacency[bPtr];
                    if(find(aAdjacency.begin(), aAdjacency.end(), bPtr) == aAdjacency.end()) {
                        adjacency[aPtr].push_back(bPtr);
                    }
                    if(find(bAdjacency.begin(), bAdjacency.end(), aPtr) == bAdjacency.end()) {
                        adjacency[bPtr].push_back(aPtr);
                    }
                }
            }
        }
    }

    // maintenant que l'on a un graphe, on cherche les cycles et les sauvegarde:
    // http://perso.ens-lyon.fr/eric.thierry/Graphes2009/nicolas-brunie.pdf 3.1
    map<vertex*, bool> visited;
    for(int i = 0; i < vertices.size(); i++) {
        auto* vertex = &vertices[i];

        // on reset les niveaux
        for(auto& v : vertices) {
            v.value = 0;
        }

        auto cycle = findCycle(vertex, adjacency);
        if(cycle) {
            cycles.push_back(move(cycle));
        }
    }


    // TODO


    // TODO: plus efficace avec cet algo? https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm
}

unique_ptr<GraphCycle> MusicLine::findCycle(vertex* start, map<vertex*, vector<vertex*>>& adjacency) {
    queue<vertex*> toVisit;
    start->value = 1;
    toVisit.push(start);
    map<vertex*, vertex*> parents;
    while(!toVisit.empty()) {
        vertex* v = toVisit.front();
        if(v->partOfCycle) {
            toVisit.pop();
            continue;
        }
        int currentValue = v->value;

        for(vertex* neighbor : adjacency[v]) {
            parents[neighbor] = v;
            if(neighbor->value >= currentValue) {
                // cycle trouvé, il manque plus qu'à reconstruire le polygone:
                return reconstructCycle(neighbor, parents);
            } else if(neighbor->value == 0) { // pas encore visité
                neighbor->value = currentValue+1;
                toVisit.push(neighbor);
            }
        }
        toVisit.pop();
    }
    // aucun cycle
    return nullptr;
}

int MusicLine::countParts() {
    updateGraph();
    return parts.size();
}

int MusicLine::countCycles() {
    updateGraph();
    return cycles.size();
}

unique_ptr<GraphCycle> MusicLine::reconstructCycle(vertex* endPoint, map<vertex*, vertex*>& parents) {
    vector<sf::Vector2f> vertices;

    vertex* vertex = endPoint;
    cout << "start" << endl;
    do {
        cout << vertex->x << ", " << vertex->y << endl;
        vertex->partOfCycle = true;
        vertices.emplace_back(vertex->x, vertex->y);
        if(parents.find(vertex) != parents.end()) {
            vertex = parents[vertex];
        } else {
            break;
        }
    } while(!vertex->partOfCycle); // tant qu'on a pas déjà rencontré cet élément du cycle

    cout << "end" << endl;
    return make_unique<GraphCycle>(vertices);
}

GraphCycle::GraphCycle(vector<sf::Vector2f> vertices): vertices(move(vertices)) {}

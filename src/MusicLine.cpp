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

    // on a un seul thread, on peut donc supposer sans soucis que les weak_ptr de parts donneront un shared_ptr non-expiré dans la suite de cette méthode

    // Attention, l'algorithme ci-dessus est sub-optimal
    // on commence par chercher les intersections des lignes
    vector<vertex> vertices;

    // on crée les arêtes du graphe
    map<vertex*, vector<vertex*>> adjacency; // TODO: multimap
    for(int i = 0;i<parts.size();i++) {
        auto& wl1 = parts[i];
        auto l1 = wl1.lock();

        // chaque ligne enregistre son point de fin, sauf pour la première qui enregistre aussi son point de départ
        if(i == 0) {
            vertices.push_back({.x = l1->startX, .y = l1->startY, .partOfCycle = false, .value = 0, .firstLine=nullptr, .secondLine = l1});
        }
        if(i < parts.size()-1) {
            auto& wl2 = parts[i+1];
            auto l2 = wl2.lock();
            if(l2) {
                vertices.push_back({.x = l1->endX, .y = l1->endY, .partOfCycle = false, .value = 0, .firstLine=l1, .secondLine = l2});
            }
        } else {
            vertices.push_back({.x = l1->endX, .y = l1->endY, .partOfCycle = false, .value = 0, .firstLine=l1, .secondLine = nullptr});
        }

        // lie les deux derniers sommets
        auto last = &vertices[vertices.size()-1];
        auto beforeLast = &vertices[vertices.size()-2];
        adjacency[last].push_back(beforeLast);
        adjacency[beforeLast].push_back(last);
    }

    // ajout des points d'intersection
    map<pair<int, int>, bool> alreadyChecked;
    for(int i = 0;i<parts.size();i++) {
        auto &wl1 = parts[i];
        auto l1 = wl1.lock();

        for(int j = 0;j<parts.size();j++) {
            auto &wl2 = parts[j];
            auto l2 = wl2.lock();

            // les segments ne doivent pas être consécutifs (ou confondus)
            if(abs(i-j) < 2) {
                continue;
            }
            int minIndex = min(i, j);
            int maxIndex = max(i, j);
            if( ! alreadyChecked[{ minIndex, maxIndex }]) {
                alreadyChecked[{ minIndex, maxIndex }] = true;
                sf::Vector2f intersectionPoint(0,0);
                if(l1->intersects(*l2, intersectionPoint)) {
                    vertices.push_back({.x = intersectionPoint.x, .y = intersectionPoint.y, .partOfCycle = false, .value = 0, .firstLine=nullptr, .secondLine=nullptr}); // origine mise à nullptr pour que connectToClosest2 ne connecte pas ce point à lui-même
                    cout << "found intersection at " << intersectionPoint.x << ", " << intersectionPoint.y << endl;
                    auto* newVertex = &vertices[vertices.size()-1];
                    connectToClosest2(newVertex, vertices, adjacency, l1);
                    connectToClosest2(newVertex, vertices, adjacency, l2);
                    newVertex->firstLine = l1;
                    newVertex->secondLine = l2;
                }
            }
        }
    }

    // maintenant que l'on a un graphe, on cherche les cycles et les sauvegarde:
    // http://perso.ens-lyon.fr/eric.thierry/Graphes2009/nicolas-brunie.pdf 3.1
    map<vertex*, bool> visited;
    for(int i = 0; i < vertices.size(); i++) {
        auto* vertex = &vertices[i];

        if(vertex->partOfCycle)
            continue;

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
    map<vertex*, vertex*> parents;
    queue<vertex*> toVisit;

    start->value = 1;
    toVisit.push(start);
    while(!toVisit.empty()) {
        vertex* v = toVisit.front();
        toVisit.pop();
        int currentValue = v->value;

        for(vertex* neighbor : adjacency[v]) {
            if(neighbor->value >= currentValue) {
                // cycle trouvé, il manque plus qu'à reconstruire le polygone:
                return reconstructCycle(v, parents);
            } else if(neighbor->value == 0) { // pas encore visité
                parents[neighbor] = v;
                neighbor->value = currentValue+1;
                toVisit.push(neighbor);
            }
        }
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

void MusicLine::connectToClosest2(vertex *v, vector<vertex> &allVertices,
                                  map<vertex *, vector<vertex *>> &adjacency, const shared_ptr<MusicLinePart>& along) {
    vertex* closest = nullptr;
    vertex* secondClosest = nullptr;
    float sqDistanceClosest = INFINITY;
    float sqDistance2ndClosest = INFINITY;
    for(int i = 0;i < allVertices.size();i++) {
        vertex* other = &allVertices[i];
        // on est bien sur la même ligne
        if(other->firstLine != along && other->secondLine != along) {
            continue;
        }

        float deltaX = other->x - v->x;
        float deltaY = other->y - v->y;
        float sqDistance = deltaX*deltaX+deltaY*deltaY;
        // plus proche que les 2 plus proches
        if(sqDistance < sqDistanceClosest) {
            secondClosest = closest;
            sqDistance2ndClosest = sqDistanceClosest;

            closest = other;
            sqDistanceClosest = sqDistance;
        } else if(sqDistance < sqDistance2ndClosest) { // seulement 2e plus proche
            secondClosest = other;
            sqDistance2ndClosest = sqDistance;
        }
    }

    assert(closest != nullptr);
    assert(secondClosest != nullptr);

    adjacency[closest].push_back(v);
    adjacency[secondClosest].push_back(v);

    adjacency[v].push_back(closest);
    adjacency[v].push_back(secondClosest);

    // brise les anciennes adjacences
    auto& adjClosest = adjacency[closest];
    adjClosest.erase(remove(adjClosest.begin(), adjClosest.end(), secondClosest), adjClosest.end());
    adjacency[secondClosest].erase(remove(adjacency[secondClosest].begin(), adjacency[secondClosest].end(), closest), adjacency[secondClosest].end());
}

GraphCycle::GraphCycle(vector<sf::Vector2f> vertices): vertices(move(vertices)) {}

//
// Created by jglrxavpok on 30/05/2020.
//

#include <elements/PlayerLineElement.h>
#include "MusicLine.h"
#include "math_help.h"
#include <queue>

MusicLine::MusicLine(std::unique_ptr<Scene>& scene, std::shared_ptr<sf::Texture> singleNotesSpritesheet, std::shared_ptr<sf::Texture> doubleNotesSpritesheet):
    scene(scene), singleNotesSpritesheet(std::move(singleNotesSpritesheet)), doubleNotesSpritesheet(std::move(doubleNotesSpritesheet)) {

}

void MusicLine::addLine(float startX, float startY, float endX, float endY) {
    auto partReference = make_shared<MusicLinePart>(startX, startY, endX, endY);
    parts.emplace_back(partReference);
    bool isSingle = rng.rand() < 0.7f;
    scene->addElement(make_unique<PlayerLineElement>(shared_from_this(), partReference, startX, startY, endX, endY, isSingle ? singleNotesSpritesheet : doubleNotesSpritesheet));
}

bool MusicLine::surrounds(unique_ptr<SceneElement>& element) {
    sf::Vector2f& pos = element->getPosition();
    for(const auto& cycle : cycles) {
        if(cycle->isInside(pos)) {
            return true;
        }
    }
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
    adjacency_map adjacency; // TODO: multimap
    for(int i = 0;i<parts.size();i++) {
        auto& wl1 = parts[i];
        auto l1 = wl1.lock();

        // chaque ligne enregistre son point de fin, sauf pour la première qui enregistre aussi son point de départ
        if(i == 0) {
            vertices.push_back({/*.index =*/ static_cast<int>(vertices.size()), /*.x =*/ l1->startX, /*.y =*/ l1->startY, /*.partOfCycle =*/ false, /*.value =*/ 0, /*.firstLine=*/nullptr, /*.secondLine =*/ l1});
        }
        if(i < parts.size()-1) {
            auto& wl2 = parts[i+1];
            auto l2 = wl2.lock();
            if(l2) {
                vertices.push_back({/*.index =*/ static_cast<int>(vertices.size()), /*.x =*/ l1->endX, /*.y =*/ l1->endY, /*.partOfCycle =*/ false, /*.value =*/ 0, /*.firstLine=*/l1, /*.secondLine =*/ l2});
            }
        } else {
            vertices.push_back({/*.index =*/ static_cast<int>(vertices.size()), /*.x =*/ l1->endX, /*.y =*/ l1->endY, /*.partOfCycle =*/ false, /*.value =*/ 0, /*.firstLine=*/l1, /*.secondLine =*/ nullptr});
        }

        // lie les deux derniers sommets
        auto last = &vertices[vertices.size()-1];
        auto beforeLast = &vertices[vertices.size()-2];
        adjacency[last->index].push_back(beforeLast->index);
        adjacency[beforeLast->index].push_back(last->index);
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
                    vertices.push_back({/*.index =*/ static_cast<int>(vertices.size()), /*.x =*/ intersectionPoint.x, /*.y =*/ intersectionPoint.y, /*.partOfCycle =*/ false, /*.value =*/ 0, /*.firstLine=*/nullptr, /*.secondLine=*/nullptr}); // origine mise à nullptr pour que connectToClosest2 ne connecte pas ce point à lui-même
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

        auto cycle = findCycle(vertex, vertices, adjacency);
        if(cycle) {
            cycles.push_back(move(cycle));
        }
    }
    // TODO: plus efficace avec cet algo? https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm
}

unique_ptr<GraphCycle> MusicLine::findCycle(vertex* start, vector<vertex>& allVertices, adjacency_map& adjacency) {
    map<vertex*, vertex*> parents;
    queue<vertex*> toVisit;

    // on reset les niveaux
    for(auto& v : allVertices) {
        v.value = 0;
        //v.partOfCycle = false;
    }

    start->value = 1;
    toVisit.push(start);
    while(!toVisit.empty()) {
        vertex* v = toVisit.front();
        toVisit.pop();
        int currentValue = v->value;

        if(v->partOfCycle)
            continue;

        for(int neighborID : adjacency[v->index]) {
            vertex* neighbor = &allVertices[neighborID];
            if(neighbor->value >= currentValue) {
                // cycle trouvé, il manque plus qu'à reconstruire le polygone:
                return reconstructCycle(neighbor, v, parents);
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

unique_ptr<GraphCycle> MusicLine::reconstructCycle(vertex* endPoint, vertex* secondParent, map<vertex*, vertex*>& parents) {
    vector<sf::Vector2f> vertices;

    vertex* path1 = endPoint;
    vertex* path2 = secondParent;
    vector<vertex*> traversed;
    while(path1 || path2) {
        if(path1) {
            traversed.push_back(path1);
            path1 = parents[path1];
        }

        if(path2) {
            traversed.insert(traversed.begin(), path2);
            path2 = parents[path2];
        }
    }

    auto endPointPosition = find(traversed.begin(), traversed.end(), endPoint);
    assert(endPointPosition != traversed.end());

    // recherche du premier élément commun
    auto begin = traversed.end();
    auto end = traversed.end();
    for (int i = 1; i < (traversed.end()-endPointPosition); i++) {
        auto it = endPointPosition+i;
        auto found = find(traversed.begin(), endPointPosition, *it);
        if(found != endPointPosition) {
            begin = found;
            end = it;
            break;
        }
    }

    assert(begin != traversed.end());
    assert(end != traversed.end());

    for(auto it = begin; it != end; it++) {
        vertex* v = *(it);
        v->partOfCycle = true;
        vertices.emplace_back(v->x, v->y);
    }

    if(vertices.size() < 2)
        return nullptr;
    return make_unique<GraphCycle>(vertices);
}

void MusicLine::connectToClosest2(vertex *v, vector<vertex> &allVertices,
                                  adjacency_map &adjacency, const shared_ptr<MusicLinePart>& along) {
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

    adjacency[closest->index].push_back(v->index);
    adjacency[secondClosest->index].push_back(v->index);

    adjacency[v->index].push_back(closest->index);
    adjacency[v->index].push_back(secondClosest->index);

    // brise les anciennes adjacences
    auto& adjClosest = adjacency[closest->index];
    auto& adj2ndClosest = adjacency[secondClosest->index];
    adjClosest.erase(remove(adjClosest.begin(), adjClosest.end(), secondClosest->index), adjClosest.end());
    adj2ndClosest.erase(remove(adj2ndClosest.begin(), adj2ndClosest.end(), closest->index), adj2ndClosest.end());
}

void MusicLine::debugRender(sf::RenderWindow &target) {
    for(auto& cycle : cycles) {
        for(auto& v : cycle->vertices) {
            sf::CircleShape s(10.0f);
            s.setFillColor(sf::Color::Red);
            s.setPosition(v.x-5.0f, v.y-5.0f);
            target.draw(s);
        }
    }
}

GraphCycle::GraphCycle(vector<sf::Vector2f> vertices): vertices(move(vertices)) {
    minX = INFINITY;
    minY = INFINITY;
    maxX = -INFINITY;
    maxY = -INFINITY;

    for(const auto& v: this->vertices) {
        if(v.x < minX) {
            minX = v.x;
        }
        if(v.y < minY) {
            minY = v.y;
        }
        if(v.x > maxX) {
            maxX = v.x;
        }
        if(v.y > maxY) {
            maxY = v.y;
        }
    }
}

bool GraphCycle::isInside(const sf::Vector2f &point) {
    // https://stackoverflow.com/a/218081
    if(point.x < minX)
        return false;
    if(point.y < minY)
        return false;
    if(point.x > maxX)
        return false;
    if(point.y > maxY)
        return false;

    // raycast pour trouver si on est à l'intérieur du cycle

    float epsilon = 1.0f; // 1 pixel
    float rayStartX = minX-epsilon;
    float rayStartY = minY-epsilon;

    int intersectionCount = 0;
    // -1+1 itérations, car le dernier point est relié au premier
    for (int i = 0; i < vertices.size(); ++i) {
        int next = (i+1) % vertices.size();
        auto& a = vertices[i];
        auto& b = vertices[next];
        sf::Vector2f intersectionPoint(0,0); // on s'en fiche mais la fonction en a besoin
        if(intersectionTest(rayStartX, rayStartY, point.x, point.y, a.x, a.y, b.x, b.y, intersectionPoint)) {
            intersectionCount++;
        }
    }
    return (intersectionCount % 2) == 1;
}

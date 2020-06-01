//
// Created by jglrxavpok on 30/05/2020.
//

#pragma once
#include <memory>
#include <iostream>
#include "Scene.h"
#include "elements/EnemyElement.h"
#include "MusicLinePart.h"

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

    void destroyEnemy(EnemyElement* enemy);

    /// mets à jour le graphe permettant de trouver les ennemis entourés
    /// Prends aussi en compte les lignes qui ont pu expirer
    void updateGraph();

public:
    explicit MusicLine(unique_ptr<Scene>& scene);
    void addLine(float startX, float startY, float endX, float endY);

    /// Renvoies 'true' ssi cette ligne de musique entoure l'élement donné (ie cette ligne crée une boucle et que
    /// l'élément est au centre de cette boucle)
    /// @param element l'élément à vérifier. N'a pas besoin d'être dans la scène
    bool surrounds(unique_ptr<SceneElement>& element);
    void destroySurroundedEnemies();

    int countParts();

    ~MusicLine() = default;
};

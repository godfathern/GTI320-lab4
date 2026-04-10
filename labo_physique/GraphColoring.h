#pragma once

/**
 * @file GraphColoring.h
 *
 * @brief Algorithme glouton pour la coloration de graphe.
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "ParticleSystem.h"
#include <vector>
#include <list>

namespace gti320
{
    typedef std::list<const Particle*> NeighborList;            // Type pour les particules voisines
    typedef std::list<int> ColorList;                           // Type pour stocker la palette de couleurs
    typedef std::vector<std::vector<int>> Partitions;           // Type pour stocker les indices des particules dans chaque partition.

    // Classe pour la coloration de graphe.
    //
    class GraphColoring
    {
    public:
        // Attribuer des couleurs à toutes les particules du système.
        //
        void color(ParticleSystem& particleSystem);

        // Retourner l'index des particules de chaque partition. (voir le type @a Partitions)
        //
        const Partitions& getPartitions() const { return m_partitions; }

    private:

        // Trouver une couleur qui n'est pas partagée par un voisine pour la particule @a p.
        //
        int findColor(const Particle& p, const std::vector<Particle>& particles, const std::vector<Spring>& springs, ColorList& C) const;

        // Retourner toutes les particules qui sont directement raccordées à la particule @a p par un ressort..
        //
        NeighborList findNeighbors(const Particle& p, const std::vector<Particle>& particles, const std::vector<Spring>& springs) const;


        Partitions m_partitions;            // Les partitions.
    };
}
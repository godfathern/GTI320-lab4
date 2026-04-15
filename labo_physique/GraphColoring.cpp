#include "GraphColoring.h"
#include "ParticleSystem.h"

using namespace gti320;

void GraphColoring::color(ParticleSystem& particleSystem)
{
    // La palette de couleurs
    ColorList C;

    std::vector<Particle>& particles = particleSystem.getParticles();
    const std::vector<Spring>& springs = particleSystem.getSprings();

    // Initialiser toutes les particules avec color = -1 
    for (Particle& p : particles)
    {
        p.color = -1;
    }

    // TODO Calculer les couleurs de toutes les particules du système. 
    //      Boucler sur chaque particule et appeler la fonction findColor.

    for (Particle& p : particles)
    {
        p.color = findColor(p, particles, springs, C);
    }


    // TODO Construire les partitions qui correspond à chaque couleur. 
    //     Les partitions sont représentées par un tableau d'indices de particules, un pour chaque couleur. 
    //     Stocker les partitions dans m_partitions.

    m_partitions.clear();
    m_partitions.resize(C.size());

    for (int i = 0; i < (int)particles.size(); ++i)
    {
        int colorIndex = particles[i].color;
        if (colorIndex != -1)
        {
            m_partitions[colorIndex].push_back(i);
        }
    }

}

int GraphColoring::findColor(const Particle& p, const std::vector<Particle>& particles, const std::vector<Spring>& springs, ColorList& C) const
{
    // TODO Trouver la première couleur de la palette C qui n'est pas attribuée à une particule voisine. 
    //      Si une couleur est introuvable, ajouter une nouvelle couleur à la palette et retournez la couleur. 
    //      Utiliser la fonction findNeighbors pour assembler une liste de particules qui sont directement connectées à la particule p par un ressort (les voisines).
    int ncolors = C.size();
    std::vector<int> count(ncolors, 0);

    auto neighbors = findNeighbors(p, particles, springs);

    for (const auto& neighbor : neighbors)
    {
        int neighborColor = neighbor->color;

        if (neighborColor != -1)
        {
            count[neighborColor]++;
        }
    }

    for (int c = 0; c < ncolors; ++c)
    {
        if (count[c] == 0)
        {
            return c;
        }
    }

    int newColor = ncolors;
    C.push_back(newColor);

    return newColor;
}

NeighborList GraphColoring::findNeighbors(const Particle& p, const std::vector<Particle>& particles, const std::vector<Spring>& springs) const
{
    NeighborList N;

    for (const Spring& s : springs)
    {
        if (&particles[s.index0] == &p)
        {
            N.push_back(&particles[s.index1]);
        }
        else if (&particles[s.index1] == &p)
        {
            N.push_back(&particles[s.index0]);
        }
    }

    return N;
}
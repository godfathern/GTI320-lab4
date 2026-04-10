#pragma once

/**
 * @file ParticleSystem.h
 *
 * @brief Système de particules de type masse-ressort
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "Matrix.h"
#include "Operators.h"
#include "Vector2d.h"
#include <vector>

namespace gti320
{

    /**
     * Classe particule 2D.
     */
    class Particle
    {
    public:
        bool fixed;     // indique si la particule est stationnaire (impossible à bouger)
        float m;       // masse
        Vector2f x;     // position
        Vector2f v;     // vélocité
        Vector2f f;     // force
        int color;      // coleur de particule

        Particle(const Vector2f& _x, const Vector2f& _v, const Vector2f& _f, float _m)
            : fixed(false), m(_m), x(_x), v(_v), f(_f), color(-1)
        { }

        Particle() : fixed(false), m(1.0), x(0, 0), v(0, 0), f(0, 0), color(-1)
        { }
    };

    /**
     * Classe ressort 2d.
     *
     * Un ressort relie toujours deux particules appelées particule 0 et particule
     * 1. Ces particules sont identifiées par leur indice dans le tableau de
     * particules du système de particule.
     */
    class Spring
    {
    public:
        int index0;     // indice de la particule 0
        int index1;     // indice de la particule 1
        float k;       // rigidité
        float l0;      // longueur au repos

        Spring(int _index0, int _index1, float _k, float _l0)
            : index0(_index0), index1(_index1), k(_k), l0(_l0)
        { }
    };

    /**
     * Classe représentant un système de particule masse-ressort 2D.
     */
    class ParticleSystem
    {

    private:
        std::vector<Particle> m_particles;      // les particules
        std::vector<Spring> m_springs;          // les ressorts

    public:
        ParticleSystem() : m_particles(), m_springs() { }

        /**
         * Supprime toutes les particules et tous les ressorts
         */
        void clear()
        {
            m_particles.clear();
            m_springs.clear();
        }

        /**
         * Ajoute une particule au système. La particule est copiée dans le tableau
         * m_particles.
         */
        void addParticle(const Particle& _particle)
        {
            m_particles.push_back(_particle);
        }

        /**
         * Ajoute un ressort au système. Le ressort est copié dans le tableau
         * m_springs.
         */
        void addSpring(const Spring& _spring) { m_springs.push_back(_spring); }

        /**
         * Calcul des forces exercées sur chacune des particules.
         */
        void computeForces();

        /**
         * Accesseurs pour les particules et les ressorts
         */
        const std::vector<Particle>& getParticles() const
        {
            return m_particles;
        }

        std::vector<Particle>& getParticles()
        {
            return m_particles;
        }

        const std::vector<Spring>& getSprings() const
        {
            return m_springs;
        }

        std::vector<Spring>& getSprings()
        {
            return m_springs;
        }

        /**
         * Assemble les vecteurs d'états.
         */
        void pack(Vector<float, Dynamic>& _pos,
            Vector<float, Dynamic>& _vel,
            Vector<float, Dynamic>& _force);

        /**
         * Mise à jour de la position et de la vitesse de chacune des particules à
         * partir des vecteurs d'état.
         */
        void unpack(const Vector<float, Dynamic>& _pos,
            const Vector<float, Dynamic>& _vel);

        /**
         * Contruit la matrice de masse.
         */
        void buildMassMatrix(Matrix<float, Dynamic, Dynamic>& M);

        /**
         * Construit la matrice df/dx
         */
        void buildDfDx(Matrix<float, Dynamic, Dynamic>& dfdx);

    };
}

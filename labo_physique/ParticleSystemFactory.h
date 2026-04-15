#pragma once

/**
 * @file ParticleSystemFactory.h
 *
 * @brief Classe utilis�e pour cr�er des syst�mes de particules.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Operators.h"
#include "Vector2d.h"
#include <vector>

namespace gti320
{
    class ParticleSystem;

    /**
     * Classe factory.
    */
    class ParticleSystemFactory
    {
    public:

        // Create a hanging cloth that is pinned at two corners.
        //
        static void createHangingCloth(ParticleSystem& particleSystem, float k);

        // Create a larger cloth example.
        //
        static void createLargeHangingCloth(ParticleSystem& particleSystem, float k);

        // Create a 2D rope pinned at the top.
        //
        static void createHangingRope(ParticleSystem& particleSystem, float k);

        // Create a beam pinned at one end.
        //
        static void createBeam(ParticleSystem& particleSystem, float k);

        // Create your own example
        //
        static void createVotreExemple(ParticleSystem& particleSystem, float k);

    };

}

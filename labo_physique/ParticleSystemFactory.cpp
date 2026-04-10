#include "ParticleSystemFactory.h"

#include "ParticleSystem.h"

using namespace gti320;

// Create a hanging cloth that is pinned at two corners.
//
void ParticleSystemFactory::createHangingCloth(ParticleSystem& particleSystem, float k)
{
    particleSystem.clear();

    const int N = 16;
    const int x_start = 240;
    const int y_start = 80;
    const int dx = 32;
    const int dy = 32;

    int index = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            const int x = x_start + j * dx;
            const int y = y_start + i * dy;

            Particle particle(Vector2f(x, y), Vector2f(0, 0), Vector2f(0, 0), 1.0);
            if (j == 0 && i == (N - 1)) particle.fixed = true;
            if (j == (N - 1) && i == (N - 1)) particle.fixed = true;
            particleSystem.addParticle(particle);
            if (i > 0)
            {
                Spring s(index - N, index, k, (float)dy);
                particleSystem.addSpring(s);
            }
            if (j > 0)
            {
                Spring s(index - 1, index, k, (float)dx);
                particleSystem.addSpring(s);
            }

            if (i > 0 && j > 0)
            {
                Spring s(index - N - 1, index, k, std::sqrt((float)dx * dx + (float)dy * dy));
                particleSystem.addSpring(s);
            }
            ++index;
        }
    }

}

// Create a larger cloth example.
//
void ParticleSystemFactory::createLargeHangingCloth(ParticleSystem& particleSystem, float k)
{
    particleSystem.clear();

    const int N = 32;
    const int x_start = 240;
    const int y_start = 80;
    const int dx = 16;
    const int dy = 16;

    int index = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            const int x = x_start + j * dx;
            const int y = y_start + i * dy;

            Particle particle(Vector2f(x, y), Vector2f(0, 0), Vector2f(0, 0), 1.0);
            if (j == 0 && i == (N - 1)) particle.fixed = true;
            if (j == (N - 1) && i == (N - 1)) particle.fixed = true;
            particleSystem.addParticle(particle);
            if (i > 0)
            {
                Spring s(index - N, index, k, (float)dy);
                particleSystem.addSpring(s);
            }
            if (j > 0)
            {
                Spring s(index - 1, index, k, (float)dx);
                particleSystem.addSpring(s);
            }

            if (i > 0 && j > 0)
            {
                Spring s(index - N - 1, index, k, std::sqrt((float)dx * dx + (float)dy * dy));
                particleSystem.addSpring(s);
            }
            ++index;
        }
    }

}

// Create a 2D rope pinned at the top.
//
void ParticleSystemFactory::createHangingRope(ParticleSystem& particleSystem, float k)
{
    particleSystem.clear();

    const int N = 20;
    const int x_start = 200;
    const int dx = 32;

    int index = 0;
    for (int j = 0; j < N; ++j)
    {
        const int x = x_start + j * dx;
        const int y = 480;

        Particle particle(Vector2f(x, y), Vector2f(0, 0), Vector2f(0, 0), 1.0);
        particle.fixed = (index == 0) || (index == N - 1);
        particleSystem.addParticle(particle);
        if (j > 0)
        {
            Spring s(index - 1, index, k, (float)dx);
            particleSystem.addSpring(s);
        }
        ++index;
    }
}

// Create a beam pinned at one end.
//
void ParticleSystemFactory::createBeam(ParticleSystem& particleSystem, float k)
{
    particleSystem.clear();

    const int N = 20;
    const int x_start = 200;
    const int y_start = 400;
    const int dx = 32;
    const int dy = 32;

    int index = 0;
    for (int j = 0; j < N; ++j)
    {
        const int x = x_start + j * dx;

        // Bottom particle
        {
            Particle particle(Vector2f(x, y_start), Vector2f(0, 0), Vector2f(0, 0), 1.0);
            particle.fixed = (j == 0);
            particleSystem.addParticle(particle);
            if (j > 0)
            {
                Spring s(index - 1, index, k, sqrt((float)dx * dx + (float)dy * dy));
                particleSystem.addSpring(s);
                Spring s2(index - 2, index, k, (float)dx);
                particleSystem.addSpring(s2);

            }
            ++index;
        }


        // Top particle
        {
            Particle particle(Vector2f(x, y_start + dy), Vector2f(0, 0), Vector2f(0, 0), 1.0);
            particle.fixed = (j == 0);
            particleSystem.addParticle(particle);
            Spring s(index - 1, index, k, (float)dy);
            particleSystem.addSpring(s);
            if (j > 0)
            {
                Spring s2(index - 2, index, k, (float)dx);
                particleSystem.addSpring(s2);
                Spring s3(index - 3, index, k, sqrt((float)dx * dx + (float)dy * dy));
                particleSystem.addSpring(s3);
            }
            ++index;
        }
    }

}


void ParticleSystemFactory::createVotreExemple(ParticleSystem& particleSystem, float k)
{
    particleSystem.clear();

    // TODO Créez votre propre exemple. Ajoutez du code ici.
    //

}
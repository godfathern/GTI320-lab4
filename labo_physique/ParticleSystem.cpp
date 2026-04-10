#include "ParticleSystem.h"
#include "SubMatrix.h"

using namespace gti320;

/**
 * Calcule des forces qui affectent chacune des particules.
 *
 * Étant donné une particule p, la force est stockée dans le vecteur p.f
 * Les forces prisent en compte sont : la gravité et la force des ressorts.
 */
void ParticleSystem::computeForces()
{
    // TODO 
    //
    // Calcul de la force gravitationnelle sur chacune des particules
    for (Particle& p : m_particles)
    {
    }

    // TODO
    //
    // Pour chaque ressort, ajouter la force exercée à chacune des exptrémités sur
    // les particules appropriées. Pour un ressort s, les deux particules
    // auxquelles le ressort est attaché sont m_particles[s.index0] et
    // m_particles[s.index1]. On rappelle que les deux forces sont de même
    // magnitude mais dans des directions opposées.
    for (const Spring& s : m_springs)
    {
    }
}

/**
 * Assemble les données du système dans les vecteurs trois vecteurs d'état _pos,
 * _vel et _force.
 */
void ParticleSystem::pack(Vector<float, Dynamic>& _pos,
    Vector<float, Dynamic>& _vel,
    Vector<float, Dynamic>& _force)
{
    // TODO 
    //
    // Copier les données des particules dans les vecteurs. Attention, si on a
    // changé de modèle, il est possible que les vecteurs n'aient pas la bonne
    // taille. Rappel : la taille de ces vecteurs doit être 2 fois le nombre de
    // particules.

}

/**
 * Copie les données des vecteurs d'états dans le particules du système.
 */
void ParticleSystem::unpack(const Vector<float, Dynamic>& _pos,
    const Vector<float, Dynamic>& _vel)
{
    // TODO 
    //
    // Mise à jour des propriétés de chacune des particules à partir des valeurs
    // contenues dans le vecteur d'état.

}



/**
 * Construction de la matrice de masse.
 */
void ParticleSystem::buildMassMatrix(Matrix<float, Dynamic, Dynamic>& M)
{
    const int numParticles = m_particles.size();
    const int dim = 2 * numParticles;
    M.resize(dim, dim);
    M.setZero();

    // TODO 
    //
    // Inscrire la masse de chacune des particules dans la matrice de masses M
    //
    for (int i = 0; i < numParticles; ++i)
    {
    }
}


/**
 * Construction de la matrice de rigidité.
 */
void ParticleSystem::buildDfDx(Matrix<float, Dynamic, Dynamic>& dfdx)
{
    const int numParticles = m_particles.size();
    const int numSprings = m_springs.size();
    const int dim = 2 * numParticles;
    dfdx.resize(dim, dim);
    dfdx.setZero();

    // Pour chaque ressort...
    for (const Spring& spring : m_springs)
    {
        // TODO
        //
        // Calculer le coefficients alpha et le produit dyadique tel que décrit au cours.
        // Utiliser les indices spring.index0 et spring.index1 pour calculer les coordonnées des endroits affectés.
        //
        // Astuce: créer une matrice de taille fixe 2 par 2 puis utiliser la classe SubMatrix pour accumuler 
        // les modifications sur la diagonale (2 endroits) et pour mettre à jour les blocs non diagonale (2 endroits).


    }
}

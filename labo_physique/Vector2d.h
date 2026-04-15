#pragma once

/**
 * @file Vector2d.h
 *
 * @brief Vecteur 2D
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "Vector.h"

namespace gti320
{

    /**
     * Spécialisation de template pour le les vecteurs 2D
     */
    template<typename _Scalar>
    class Vector<_Scalar, 2> : public MatrixBase<_Scalar, 2, 1>
    {
    public:

        /**
         * Constructeur par défaut
         */
        Vector() : MatrixBase<_Scalar, 2, 1>() { }

        /**
         * Constructeur à partir de la dimension. Afin d'assurer la compatibilité
         * avec la version non-spécialisée de la classe.
         *
         * @param _size doit être 2
         */
        Vector(int _size) : MatrixBase<_Scalar, 2, 1>()
        {
            assert(_size == 2);
        }

        /**
         * Constructeur à partir des coordonnées x et y
         */
        Vector(_Scalar x, _Scalar y) : MatrixBase<_Scalar, 2, 1>()
        {
            (*this)(0) = x;
            (*this)(1) = y;
        }

        /**
         * Constructeur de copie
         */
        Vector(const Vector& other) : MatrixBase<_Scalar, 2, 1>(other) { }

        /**
         * Destructeur
         */
        ~Vector() { }

        /**
         * Opérateur de copie
         */
        Vector& operator=(const Vector& other)
        {
            this->m_storage = other.m_storage;
            return *this;
        }

        /**
         * Accesseur à une entrée du vecteur (lecture seule)
         */
        _Scalar operator()(int i) const
        {
            assert(0 <= i && i < 2);
            return *(this->m_storage.data() + i);
        }

        /**
         * Accesseur à une entrée du vecteur (lecture et écriture)
         */
        _Scalar& operator()(int i)
        {
            assert(0 <= i && i < 2);
            return *(this->m_storage.data() + i);
        }

        /**
         * Produit scalaire de *this et other
         */
        inline _Scalar dot(const Vector& other) const
        {
            return (*this)(0) * other(0) + (*this)(1) * other(1);
        }

        /**
         * Norme eulidienne du vecteur
         */
        inline _Scalar norm() const
        {
            return sqrt(dot(*this));
        }

        /** Nouvelle
         * Opérateur de division par un scalaire
         */
        inline Vector operator/(_Scalar s) const
        {
            assert(s > 0); // division par zéro
            return Vector((*this)(0) / s, (*this)(1) / s);
        }

        /** Nouvelle
        * Retourne la transposee du vecteur
        */
        inline Matrix<_Scalar, 1, 2> transpose() const
        {
            Matrix<_Scalar, 1, 2> result;
            result(0, 0) = (*this)(0);
            result(0, 1) = (*this)(1);
            return result;
        }
    };

    /**
    * Operateur de multiplication entre un vecteur colonne
    * et un vecteur ligne pour produire une matrice.
    */

    template<typename _Scalar>
    inline Matrix<_Scalar, 2, 2> operator*(const Vector<_Scalar, 2>& u, const Matrix<_Scalar, 1, 2>& v)
    {
        Matrix<_Scalar, 2, 2> res;
        res(0, 0) = u(0) * v(0, 0);
        res(0, 1) = u(0) * v(0, 1);
        res(1, 0) = u(1) * v(0, 0);
        res(1, 1) = u(1) * v(0, 1);
        return res;
    }

    typedef Vector<float, 2> Vector2f;
    typedef Vector<double, 2> Vector2d;
}

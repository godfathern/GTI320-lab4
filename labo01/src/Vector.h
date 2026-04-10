#pragma once

/**
 * @file Vector.h
 *
 * @brief Implémentation de vecteurs simples et denses.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include <cmath>
#include "MatrixBase.h"

namespace gti320 {

    /**
     * Classe vecteur générique.
     *
     * Cette classe réutilise la classe `MatrixBase` et ses spécialisations de
     * templates pour les manipulation bas niveau.
     */
    template <typename _Scalar = double, int _Rows = Dynamic>
    class Vector : public MatrixBase<_Scalar, _Rows, 1> {
    public:

        /**
         * Constructeur par défaut
         */
        Vector() : MatrixBase<_Scalar, _Rows, 1>() {}

        /**
         * Contructeur à partir d'un taille (rows).
         */
        explicit Vector(int rows) : MatrixBase<_Scalar, _Rows, 1>(rows, 1) {}

        /**
         * Constructeur de copie
         */
        Vector(const Vector& other) : MatrixBase<_Scalar, _Rows, 1>(other) {}

        /**
         * Destructeur
         */
        ~Vector() {}

        /**
         * Accesseur à une entrée du vecteur (lecture seule)
         */
        _Scalar operator()(int i) const
        {
            // TODO implémenter
            return this->data()[i];
        }

        /**
         * Accesseur à une entrée du vecteur (lecture et écriture)
         */
        _Scalar& operator()(int i)
        {
            // TODO implémenter
            return this->data()[i];
        }

        /**
         * Modifie le nombre de lignes du vecteur
         */
        void resize(int _rows)
        {
            MatrixBase<_Scalar, _Rows, 1>::resize(_rows, 1);
        }

        /**
         * Produit scalaire de *this et other.
         */
        inline _Scalar dot(const Vector& other) const
        {
            // TODO implémenter
            int rows = this->rows();
            _Scalar result = _Scalar(0);

            assert(rows == other.rows());

            for (int i = 0; i < rows; ++i) {
                result += this->data()[i] * other.data()[i];
            }


            return result;
        }

        /**
         * Retourne la norme euclidienne du vecteur
         */
        inline _Scalar norm() const
        {
            // TODO implémenter
            int rows = this->rows();
            _Scalar resultCaree = _Scalar(0);
            for (int i = 0; i < rows; ++i) {
                _Scalar v = this->data()[i];
                resultCaree += v * v;
            }

            return std::sqrt(resultCaree);
        }
    };
}

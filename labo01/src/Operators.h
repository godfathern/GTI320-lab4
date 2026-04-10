#pragma once

/**
 * @file Operators.h
 *
 * @brief Opérateurs arithmétiques pour les matrices et les vecteurs.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Vector.h"
#include "SparseMatrix.h"
#include <algorithm>

 /**
  * Implémentation de divers opérateurs arithmétiques pour les matrices et les vecteurs.
  */
namespace gti320 {

    /**
     * Multiplication : Matrice * Matrice (générique) - testé
     */
    template <typename _Scalar, int RowsA, int ColsA, int StorageA, int RowsB, int ColsB, int StorageB>
    Matrix<_Scalar, RowsA, ColsB> operator*(const Matrix<_Scalar, RowsA, ColsA, StorageA>& A, const Matrix<_Scalar, RowsB, ColsB, StorageB>& B)
    {
        // TODO implémenter

        int rowsA = A.rows();
        int colsA = A.cols();

        int rowsB = B.rows();
        int colsB = B.cols();

        assert(colsA == rowsB);

        Matrix<_Scalar, RowsA, ColsB> result(rowsA, colsB);

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                _Scalar sum = _Scalar(0);
                for (int k = 0; k < colsA; ++k) {
                    sum += A(i, k) * B(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    /**
     * Multiplication : Matrice (colonne) * Matrice (ligne)
     *
     * Spécialisation de l'opérateur de multiplication pour le cas où les matrices
     * ont un stockage à taille dynamique et où la matrice de gauche utilise un
     * stockage par colonnes et celle de droite un stockage par lignes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator*(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& B)
    {
        // TODO : implémenter
        const int rowsA = A.rows();
        const int colsA = A.cols();
        const int rowsB = B.rows();
        const int colsB = B.cols();

        assert(colsA == rowsB);

        Matrix<_Scalar, Dynamic, Dynamic> C(rowsA, colsB);
        C.setZero();

        for (int i = 0; i < rowsA; ++i) {
            for (int k = 0; k < colsA; ++k) {
                const _Scalar a = A(i, k);
                for (int j = 0; j < colsB; ++j) {
                    C(i, j) += a * B(k, j);
                }
            }
        }

        return C;
    }

    /**
     * Multiplication : Matrice (ligne) * Matrice (colonne)
     *
     * Spécialisation de l'opérateur de multiplication pour le cas où les matrices
     * ont un stockage à taille dynamique et où la matrice de gauche utilise un
     * stockage par lignes et celle de droite un stockage par colonnes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator*(const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        // TODO : implémenter
        const int rowsA = A.rows();
        const int colsA = A.cols();
        const int rowsB = B.rows();
        const int colsB = B.cols();

        assert (colsA == rowsB);

        Matrix<_Scalar, Dynamic, Dynamic> C(rowsA, colsB);
        C.setZero();

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                _Scalar sum = _Scalar(0);
                for (int k = 0; k < colsA; ++k) {
                    sum += A(i, k) * B(k, j);
                }
                C(i, j) = sum;
            }
        }

        return C;
    }


    /**
     * Addition : Matrice + Matrice (générique) - teste
     */
    template <typename _Scalar, int Rows, int Cols, int StorageA, int StorageB>
    Matrix<_Scalar, Rows, Cols> operator+(const Matrix<_Scalar, Rows, Cols, StorageA>& A, const Matrix<_Scalar, Rows, Cols, StorageB>& B)
    {
        // TODO : implémenter
        const int rowsA = A.rows();
        const int colsA = A.cols();
        const int rowsB = B.rows();
        const int colsB = B.cols();

        assert (rowsA == rowsB);
        assert (colsA == colsB);

        Matrix <_Scalar, Rows, Cols> C(rowsA, colsA);

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsA; ++j) {
                C(i,j) = A(i,j) + B(i,j);
            }
        }


        return C;
    }

    /**
     * Addition : Matrice (colonne) + Matrice (colonne)
     *
     * Spécialisation de l'opérateur d'addition pour le cas où les deux matrices
     * sont stockées par colonnes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic> operator+(const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, ColumnStorage>& B)
    {
        // TODO : implémenter
        const int rowsA = A.rows();
        const int colsA = A.cols();
        const int rowsB = B.rows();
        const int colsB = B.cols();

        assert (rowsA == rowsB);
        assert (colsA == colsB);

        Matrix <_Scalar, Dynamic, Dynamic> C(rowsA, colsA);

        const _Scalar*  a = A.data();
        const _Scalar*  b = B.data();
        _Scalar* c = C.data();


        const int n = rowsA * colsA;
        for (int i = 0; i < n; ++i) {
            c[i] = a[i] + b[i];
        }
        return C;
    }




    /**
     * Addition : Matrice (ligne) + Matrice (ligne)
     *
     * Spécialisation de l'opérateur d'addition pour le cas où les deux matrices
     * sont stockées par lignes.
     */
    template <typename _Scalar>
    Matrix<_Scalar, Dynamic, Dynamic, RowStorage> operator+(const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& A, const Matrix<_Scalar, Dynamic, Dynamic, RowStorage>& B)
    {
        // TODO : implémenter

        int rows = A.rows();
        int cols = A.cols();

        assert(rows == B.rows());
        assert(cols == B.cols());

        Matrix<_Scalar, Dynamic, Dynamic, RowStorage> C(rows,cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                C(i,j) = A(i,j) + B(i,j);
            }
        }

        return C;
    }

    /**
     * Multiplication  : Scalaire * Matrice (colonne) - testé
     *
     * Spécialisation de l'opérateur de multiplication par un scalaire pour le
     * cas d'une matrice stockée par colonnes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Matrix<_Scalar, _Rows, _Cols, ColumnStorage> operator*(const _Scalar& a, const Matrix<_Scalar, _Rows, _Cols, ColumnStorage>& A)
    {
        // TODO : implémenter
        const int rows = A.rows();
        const int cols = A.cols();
        Matrix<_Scalar, _Rows, _Cols, ColumnStorage> result(rows, cols);
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                result(i,j) = a * A(i,j);
            }
        }
        return result;
    }

    /**
     * Multiplication  : Scalaire * Matrice (ligne)
     *
     * Spécialisation de l'opérateur de multiplication par un scalaire pour le
     * cas d'une matrice stockée par lignes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Matrix<_Scalar, _Rows, _Cols, RowStorage> operator*(const _Scalar& a, const Matrix<_Scalar, _Rows, _Cols, RowStorage>& A)
    {
        // TODO : implémenter
        const int rows = A.rows();
        const int cols = A.cols();
        Matrix<_Scalar, _Rows, _Cols, RowStorage> result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(i, j) = a * A(i, j);
            }
        }
        return result;
    }

    /**
     * Multiplication : Matrice (ligne) * Vecteur
     *
     * Spécialisation de l'opérateur de multiplication matrice*vecteur pour le
     * cas où la matrice est représentée par lignes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const Matrix<_Scalar, _Rows, _Cols, RowStorage>& A, const Vector<_Scalar, _Cols>& v)
    {
        // TODO : implémenter
        const int rows = A.rows();
        const int cols = A.cols();
        assert(cols == v.rows());
        Vector<_Scalar, _Rows> result(rows);
        result.setZero();
        for (int i = 0; i < rows; ++i) {
            _Scalar sum = _Scalar(0);
            for (int j = 0; j < cols; ++j) {
                sum += v(j) * A(i,j);
            }
            result(i) = sum;
        }
        return result;
    }

    /**
     * Multiplication : Matrice (colonne) * Vecteur
     *
     * Spécialisation de l'opérateur de multiplication matrice*vecteur pour le
     * cas où la matrice est représentée par colonnes.
     */
    template <typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const Matrix<_Scalar, _Rows, _Cols, ColumnStorage>& A, const Vector<_Scalar, _Cols>& v)
    {
        // TODO : implémenter
        const int rows = A.rows();
        const int cols = A.cols();
        assert(cols == v.rows());
        Vector<_Scalar, _Rows> result(rows);
        result.setZero();
        for (int j = 0; j < cols; ++j) {
            const _Scalar x = v(j);
            for (int i = 0; i < rows; ++i) {
                result(i) += A(i, j) * x;
            }
        }

        return result;
    }

    /**
     * Multiplication : Scalaire * Vecteur
     */
    template <typename _Scalar, int _Rows>
    Vector<_Scalar, _Rows> operator*(const _Scalar& a, const Vector<_Scalar, _Rows>& v)
    {
        // TODO : implémenter
        const int rows = v.rows();
        Vector<_Scalar, _Rows> result(rows);

        for (int i = 0; i < rows; ++i) {
            result(i) = v(i) * a;
        }

        return result;
    }


    /**
     * Addition : Vecteur + Vecteur
     */
    template <typename _Scalar, int _RowsA, int _RowsB>
    Vector<_Scalar, _RowsA> operator+(const Vector<_Scalar, _RowsA>& a, const Vector<_Scalar, _RowsB>& b)
    {
        // TODO : implémenter
        const int rows = a.rows();
        assert (rows == b.rows());

        Vector<_Scalar, _RowsA> result(rows);
        for (int i = 0; i < rows; ++i) {
            result(i) = a(i) + b(i);
        }
        return result;
    }

    /**
     * Soustraction : Vecteur - Vecteur
     */
    template <typename _Scalar, int _RowsA, int _RowsB>
    Vector<_Scalar, _RowsA> operator-(const Vector<_Scalar, _RowsA>& a, const Vector<_Scalar, _RowsB>& b)
    {
        // TODO : implémenter
        const int rows = a.rows();
        assert (rows == b.rows());

        Vector<_Scalar, _RowsA> result(rows);
        for (int i = 0; i < rows; ++i) {
            result(i) = a(i) - b(i);
        }
        return result;
    }
    

    /**
     * Multiplication : SparseMatrix * Vecteur : slide 21 (page 22 du cours 3 a appliquer), eviter de call operator() ici (big-o va augmenter insanely
     */
    template<typename _Scalar, int _Rows, int _Cols>
    Vector<_Scalar, _Rows> operator*(const SparseMatrix<_Scalar, _Cols, _Rows>& A, const Vector<_Scalar, _Cols>& v)
    {
        const int m = (int)A.rows();
        const int n = (int)A.cols();

        assert(n == v.rows());

        Vector<_Scalar, _Rows> y(m);
        y.setZero();

        for (unsigned int i = 0; i < (unsigned int)m; ++i)
        {
            const unsigned int begin = A.outer()[i];
            const unsigned int end = (i + 1 < (unsigned int)m) ? A.outer()[i + 1] : (unsigned int)A.getInnerSize();

            _Scalar sum = _Scalar(0);

            for (unsigned int k = begin; k < end; ++k)
            {
                const unsigned int j = A.inner()[k];
                sum += A.values()[k] * v((int)j);
            }

            y((int)i) = sum;
        }

        return y;
    }

}

#pragma once

/**
 * @file SparseMatrix.h
 *
 * @brief Une impl�mentation simple de matrice creuse.
 *        Note:  actuellement, seules les matrices dynamiques
 *        sont possibles.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "SparseMatrixBase.h"

#include <cstring>
#include <cassert>
#include <vector>

namespace gti320
{

    // Matrice creuse de type compressed row storage (CRS) sparse matrix avec taille dynamique.
    //
    template <typename _Scalar = double, int _ColsAtCompile = Dynamic, int _RowsAtCompile = Dynamic>
    class SparseMatrix : public SparseMatrixBase<_Scalar, _ColsAtCompile, _RowsAtCompile>
    {
    private:
        unsigned int m_rows, m_cols;
    public:

        // Constructeur par d�faut
        SparseMatrix() :
            SparseMatrixBase<_Scalar, Dynamic, Dynamic>(),
            m_rows(0), m_cols(0)
        { }

        // Constructeur de copie
        SparseMatrix(const SparseMatrix& other) :
            SparseMatrixBase<_Scalar, Dynamic, Dynamic>(other),
            m_rows(other.m_rows), m_cols(other.m_cols)
        { }

        // Constructeur avec des dimensions
        explicit SparseMatrix(int _rows, int _cols) :
            SparseMatrixBase<_Scalar, Dynamic, Dynamic>(_rows, 0),
            m_rows(_rows), m_cols(_cols)
        { }

        // Destructeur
        ~SparseMatrix() { }


        // Il faut cette fonction
        // TODO access operator (read-only)
        _Scalar operator()(int i, int j) const
        {
            // TODO : impl�menter
            assert(i >= 0 && i < m_rows);
            assert(j >= 0 && j < m_cols);

            const  int row = i;
            const int col = j;

            const unsigned int begin = this->m_start[row];
            int end;

            if (row + 1 < m_rows) {
                 end = this->m_start[row + 1];
            }
            else {
                end = this->m_vals.size();
            }

            for (int k = begin; k < end; ++k)
            {
                if (this->m_inner[k] == col)
                    return this->m_vals[k];
            }

            return 0.0;
        }

        unsigned int rows() const { return m_rows; }

        unsigned int cols() const { return m_cols; }

        // Set this matrix to the identity matrix.
        void setIdentity()
        {
            // TODO mettre toutes les valeurs � 0,0, sauf pour la diagonale qui est � 1,0
            //      NB : confirmer que la matrice est carr�e

            assert(m_rows == m_cols);

            const unsigned int n = m_rows;

            this->m_vals.resize(n);
            this->m_inner.resize(n);
            this->m_start.resize(n);
            this->m_start.setZero();

            for (unsigned int i = 0; i < n; ++i)
            {
                this->m_start[i] = i;
                this->m_vals[i] = _Scalar(1);
                this->m_inner[i] = i;
            }

        }

        void setFromTriplets(TripletType<_Scalar>* _triplets, unsigned int _size) {
            assert((_triplets != nullptr) || (_size == 0));

            this->m_vals.resize(_size);
            this->m_inner.resize(_size);
            this->m_start.resize(m_rows);
            this->m_start.setZero();

            if (m_rows == 0 || m_cols == 0 || _size == 0)
                return;

            for (unsigned int r = 0; r < m_rows; ++r)
            {
                unsigned int countBefore = 0;

                for (unsigned int t = 0; t < _size; ++t)
                {
                    const unsigned int row = _triplets[t].i;
                    const unsigned int col = _triplets[t].j;

                    assert(row < m_rows);
                    assert(col < m_cols);

                    if (row < r)
                        countBefore++;
                }

                this->m_start[r] = countBefore;
            }

            unsigned int pos = 0;

            for (unsigned int r = 0; r < m_rows; ++r)
            {
                for (unsigned int t = 0; t < _size; ++t)
                {
                    const unsigned int row = _triplets[t].i;
                    const unsigned int col = _triplets[t].j;

                    assert(row < m_rows);
                    assert(col < m_cols);

                    if (row == r)
                    {
                        assert(pos < _size);

                        this->m_vals[pos]  = _triplets[t].val;
                        this->m_inner[pos] = col;
                        pos++;
                    }
                }
            }

            assert(pos == _size);
        }

    };

}

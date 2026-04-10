#pragma once

/**
 * @file SparseMatrixBase.h
 *
 * @brief Classe de base pour les matrices creuses.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "DenseStorage.h"

namespace gti320
{
    // Classe de base pour une matrice creuse.
    //
    template <typename _ScalarType, int _OuterSize, int _InnerSize>
    class SparseMatrixBase
    {
    protected:
        DenseStorage<_ScalarType, _InnerSize> m_vals;       // Stocke les coefficients de valeur non zéro
        DenseStorage<unsigned int, _InnerSize> m_inner;     // Stocke les indices de colonne des coefficients non zéro
        DenseStorage<unsigned int, _OuterSize> m_start;     // Stocke pour chaque ligne l'array index du premier élément non zéro dans m_vals et m_inner

    public:

        // Default constructor
        SparseMatrixBase() : m_vals(), m_inner(), m_start() { }

        // Copy constructor
        SparseMatrixBase(const SparseMatrixBase& other) : m_vals(other.m_vals), m_inner(other.m_inner), m_start(other.m_start) { }

        // Parameter constructor
        SparseMatrixBase(int _outerSize, unsigned int _innerSize)
        {
            // TODO : implémenter
            m_inner.resize(_innerSize);
            m_vals.resize((_innerSize));
            m_start.resize(_outerSize);
            m_start.setZero();


        }

        // Destructor
        ~SparseMatrixBase() { }

        // Assignment operator
        SparseMatrixBase& operator=(const SparseMatrixBase& other)
        {
            if (this != &other)
            {
                m_vals = other.m_vals;
                m_inner = other.m_inner;
                m_start = other.m_start;
            }
            return *this;
        }

        void setInnerSize(unsigned int _nnz)
        {
            // TODO : implémenter
            m_inner.resize(_nnz);
            m_vals.resize(_nnz);
        }

        // Number of elements
        inline unsigned int getInnerSize() const
        {
            return m_inner.size();
        }

        void setOuterSize(unsigned int _outerSize) 
        { 
            // TODO : implémenter
            m_start.resize(_outerSize);
        }

        unsigned int getOuterSize() const
        {
            return m_start.size();
        }

        inline void setZero()
        {
            // TODO : implémenter
            m_inner.setZero();
            m_start.setZero();
            m_vals.setZero();
        }

        // Access to the @a m_vals buffer (read only)
        const _ScalarType* values() const
        {
            return m_vals.data();
        }

        // Access to the @a m_start buffer (read only)
        const unsigned int* outer() const
        {
            return m_start.data();
        }

        // Access to the @a m_inner buffer (read only)
        const unsigned int* inner() const
        {
            return m_inner.data();
        }

    };

}

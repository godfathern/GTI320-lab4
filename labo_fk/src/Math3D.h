#pragma once

/**
 * @file Math3D.h
 *
 * @brief Fonctions pour l'intinialisation et la manipulation de matrices de
 * rotation, des matrices de transformations en coordonn�es homog�nes et les
 * vecteurs 3D.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */

#include "Matrix.h"
#include "Vector.h"
#include "Operators.h"
#include "SubMatrix.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 
#endif

#include <math.h>

namespace gti320 {

    // Type de vecteur 3D.
    typedef Vector<float, 3> Vector3f;

    //Nouvelle : helper pour produit vectoriel de 2 vecteurs :
    inline Vector3f crossP(const Vector3f& a, const Vector3f& b) {
        Vector3f c;
        c(0) = a(1) * b(2) - a(2) * b(1);
        c(1) = a(2) * b(0) - a(0) * b(2);
        c(2) = a(0) * b(1) - a(1) * b(0);
        return c;
    }

    // Dans le cadre de ce projet, nous consid�rons seulement deux
    // cas :
    //
    //  - les rotations
    //  - les translations
    //
    // Matrice en coordonn�es homog�es :
    typedef Matrix<float, 4, 4, ColumnStorage> Matrix4f;
    // 
    // Matrice pour les rotations
    typedef Matrix<float, 3, 3, ColumnStorage> Matrix3f;

    /**
     * Initialise et retourne la matrice identit�
     */
    template<>
    inline void Matrix4f::setIdentity()
    {
        // TODO affecter la valeur 0.0 partout, sauf sur la diagonale principale o� c'est 1.0.
        //      Note: ceci est une red�finition d'une fonction membre!
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                (*this)(i, j) = (i == j) ? 1: 0;
            }
        }
    }

    /**
     * Calcul de la matrice inverse, SP�CIALIS� pour le cas d'une matrice de
     * transformation rigide en coordonn�es homog�nes.
     *
     * TODO (vous pouvez supposer qu'il s'agit d'une matrice de transformation
     * en coordonn�es homog�nes)
     */
    template<>
    inline Matrix4f Matrix4f::inverse() const
    {
        // TODO : impl�menter
        //Matrice de rotation
        Matrix3f R = (*this).block(0,0,3,3);
        //Transposee de matrice de rotation
        Matrix3f RT = R.transpose<float, 3, 3, ColumnStorage>();
        //Matricde de transalation
        Vector3f t;

        Matrix4f result;

        //Calculer la matrice de translation
        for (int i = 0; i < 3; ++i) {
            t(i) = (*this)(i,3);
        }

        //Calculer -RT * t
        Vector3f RTt;

        for (int i = 0; i < 3; ++i)
        {
            float acc = 0.0f;

            for (int k = 0; k < 3; ++k)
            {
                acc += RT(i, k) * t(k);
            }

            RTt(i) = -acc;
        }

        //Fill in the 0 0 0 1
        for (int j = 0; j < 4; ++j) {
            result(3,j) = (j != 3) ? 0.0f : 1.0f;
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result(i,j) = RT(i,j);
            }
        }

        for (int i = 0; i < 3; ++i){
            result(i,3) = RTt(i);

        }

        return result;
    }
    
    /**
     * Calcul de la matrice inverse, SP�CIALIS� pour le cas d'une matrice de rotation.
     *
     * (vous pouvez supposer qu'il s'agit d'une matrice de rotation)
     */
    template<>
    inline Matrix3f Matrix3f::inverse() const
    {
        // TODO : impl�menter
        Matrix3f Rt;
        Rt = (*this).transpose<float, 3, 3, ColumnStorage>(); // Matrice de rotation => Orthogonale =>  inverse = transpose

        return Rt;
    }


    /**
     * Multiplication d'une matrice 4x4 avec un vecteur 3D o� la matrice
     * repr�sente une transformation rigide en coordonn�es homog�ne.
     */
    template <typename _Scalar>
    Vector<_Scalar, 3> operator*(const Matrix<_Scalar, 4, 4, ColumnStorage>& A, const Vector<_Scalar, 3>& v)
    {
        // TODO : impl�menter
        Vector<_Scalar, 3> result;

        for (int i = 0; i < 3; ++i) {
            _Scalar s = 0;
            for (int j = 0; j < 3; ++j) {
                s += A(i, j) * v(j);
            }
            s += A(i, 3);
            result(i) = s;
        }

        return result;
    }


    /**
     * Initialise et retourne la matrice de rotation d�finie par les angles
     * d'Euler XYZ exprim�s en radians.
     *
     * La matrice doit correspondre au produit : Rz*Ry*Rx.
     */
    template<typename _Scalar>
    static Matrix<_Scalar, 3, 3> makeRotation(_Scalar x, _Scalar y, _Scalar z)
    {
        // TODO : impl�menter
        //Diapo 17
        const _Scalar cos_x = cos(x), sin_x = sin(x);
        const _Scalar cos_y = cos(y), sin_y = sin(y);
        const _Scalar cos_z = cos(z), sin_z = sin(z);

        Matrix<_Scalar, 3, 3> Rx, Ry, Rz;
        //Diapo 16 + 29
        // Rx
        Rx(0,0)=1;  Rx(0,1)=0;      Rx(0,2)=0;
        Rx(1,0)=0;  Rx(1,1)=cos_x;  Rx(1,2)=-sin_x;
        Rx(2,0)=0;  Rx(2,1)=sin_x;  Rx(2,2)=cos_x;

        // Ry
        Ry(0,0)=cos_y;  Ry(0,1)=0;  Ry(0,2)=sin_y;
        Ry(1,0)=0;      Ry(1,1)=1;  Ry(1,2)=0;
        Ry(2,0)=-sin_y; Ry(2,1)=0;  Ry(2,2)=cos_y;

        // Rz
        Rz(0,0)=cos_z;  Rz(0,1)=-sin_z; Rz(0,2)=0;
        Rz(1,0)=sin_z;  Rz(1,1)=cos_z;  Rz(1,2)=0;
        Rz(2,0)=0;      Rz(2,1)=0;      Rz(2,2)=1;

        // Produit: Rz * Ry * Rx
        return Rz * (Ry * Rx);
    }

}

//
// Created by Phan Tung Bui on 2026-04-10.
//

#include <vector>
#include <cmath>

namespace Jacobi {
    typedef std::vector<double> Vector;

    inline Vector multiply1D(const Vector& A, const Vector& x, int numCols) {
        int numRows = A.size() / numCols;
        Vector result(numRows, 0.0);

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                // Accès à la matrice aplatie : i * largeur + j
                result[i] += A[i * numCols + j] * x[j];
            }
        }
        return result;
    }

    inline double calculateNorm(const Vector& v) {
        double sum = 0.0;
        for (double val : v) {
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    inline Vector doJacobi(Vector A, Vector b, Vector x0, int maxIteration, Vector& r_out, int size) {

        int n = size; //matrice n x n
        Vector r(maxIteration);
        Vector x = x0;
        Vector z(size, 0.0f); //vecteur temporaire pour garder la valeur de x[i]

        r_out.clear();
        r_out.reserve(maxIteration);

        for (int k = 0; k < maxIteration; ++k) {
            z = b;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < i; ++j) {
                    z[i] = z[i] - A[n*i + j] * x[j];
                }
// On calcul pas la diagonale (x[i] quand i = j)
                for (int j  = i + 1 ; j < n; ++j) {
                    z[i] = z[i] - A[n*i + j] * x[j];
                }
                z[i] = z[i] / A[n*i + i];
            }
            x = z;
            std::cout << "Iteration " << k << " x=" << x[0] << ", y=" << x[1] << ", z=" << x[2] << std::endl;
            r[k] = calculateNorm(multiply1D(A, x, n));
            r_out.push_back(r[k]);
        }
        return x;
    }

}


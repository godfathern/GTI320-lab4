/**
 * @file main.cpp
 *
 * @brief Exécuter des tests unitaires pour une librairie d'algèbre linéaire simple.
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	// Executer tous les tests unitaires.
	// 
	// Les tests sont écrites dans les fichiers:
	//   tests/TestsDenseStorage.cpp
	//   tests/TestsMatrix.cpp
	//   tests/TestsOperators.cpp
	//   tests/TestsPerformance.cpp
	//   tests/TestsSparseMatrix.cpp
	//   tests/TestsSupplementaires.cpp
	//   tests/TestsVector.cpp
	//
	::testing::InitGoogleTest(&argc, argv);
	const int ret = RUN_ALL_TESTS();

	return ret;
}

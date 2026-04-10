/**
 * @file main.cpp
 *
 * @brief GTI320 Labo 2 - creates a NanoGUI application.
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include "FKApplication.h"

#if defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#if defined(_WIN32)
#  pragma warning(push)
#  pragma warning(disable: 4457 4456 4005 4312)
#endif

#if defined(_WIN32)
#  pragma warning(pop)
#endif
#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    // Executer tous les tests unitaires.
    // 
    // Les tests sont écrites dans les fichiers:
    //   tests/TestsMath3D.cpp
    //   tests/TestsSubMatrix.cpp
    //
    ::testing::InitGoogleTest(&argc, argv);
    const int ret = RUN_ALL_TESTS();


    nanogui::init();
    nanogui::ref<FKApplication> app = new FKApplication();
    app->draw_all();
    app->set_visible(true);
    nanogui::run(nanogui::RunMode::Eager);
    nanogui::shutdown();

    return 0;
}
/**
 * @file main.cpp  
 *
 * @brief GTI320 Simulation d'un système masse-ressort
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */

#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/formhelper.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ParticleSimApplication.h"

// Includes for the GLTexture class.
#include <cstdint>
#include <memory>
#include <utility>
#include <ctime>

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


int main(int argc, char** argv) 
{
    nanogui::init();
    nanogui::ref<ParticleSimApplication> app = new ParticleSimApplication();
    app->draw_all();
    app->set_visible(true);
    nanogui::run(nanogui::RunMode::Eager);
    nanogui::shutdown();

    return 0;
}
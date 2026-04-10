#pragma once

/**
 * @file FKApplication.h
 *
 * @brief Application class for forward kinematics lab.
 *
 * Nom:
 * Code permanent :
 * Email :
 *
 */


#include <nanogui/window.h>
#include <nanogui/textbox.h>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/slider.h>
#include <nanogui/screen.h>
#include <nanogui/vscrollpanel.h>

#include "Math3D.h"

#include <memory>

class FKGLCanvas;
namespace gti320
{
    class Armature;
}

class LinkUI;

// Iterative closest point algorithm
//
class FKApplication : public nanogui::Screen
{
public:
	FKApplication();

    virtual bool keyboard_event(int key, int scancode, int action, int modifiers) override;
    
    virtual void draw(NVGcontext *ctx) override;

    nanogui::Window* getWindow() { return m_window.get(); }

    gti320::Armature* getArmature() const { return m_armature.get(); }

private:

    // Reset the source transform and the initial transform.
    // 
    void reset();
    void initializeArmature(const char* bvhFilename); 

    std::unique_ptr<gti320::Armature> m_armature;
    std::unique_ptr<FKGLCanvas> m_canvas;
    nanogui::ref<nanogui::Window> m_window;
    nanogui::ref<nanogui::Widget> m_links;
    std::vector< std::unique_ptr<LinkUI> > m_linkUIArr;
};
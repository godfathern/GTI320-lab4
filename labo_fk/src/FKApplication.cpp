#include "FKApplication.h"
#include "FKGLCanvas.h"
#include "BVHParser.h"

#include <nanogui/window.h>
#include <nanogui/formhelper.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <GLFW/glfw3.h>
#include <memory>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Armature.h"
#include "LinkUI.h"

using namespace nanogui;
using namespace gti320;

namespace
{
    static void createLinkSliders(Armature* armature, Widget* widget, std::vector< std::unique_ptr<LinkUI> >& linkUIArr)
    {
        while (!widget->children().empty())
        {
            Widget* child = widget->children().back();
            widget->remove_child(child);   // or delete child; depending on your branch
        }
        linkUIArr.clear();

        const int numLinks = armature->links.size();
        for (int i = 0; i < numLinks; ++i)
        {
            Link* link = armature->links[i];
            if (!link->isEndEffector())
            {
                linkUIArr.push_back(std::make_unique<LinkUI>(armature, armature->links[i], i, widget));
            }
        }

    }
}

FKApplication::FKApplication() : Screen(Vector2i(1280, 1024), "GTI320 Labo sur la cinematique directe"),
    m_armature(), m_canvas(), m_window(), m_linkUIArr()
{
    initializeArmature("resources/simple.bvh");

    m_window = new Window(this, "Cinematique directe");
    m_window->set_position(Vector2i(8, 8));
    m_window->set_layout(new GroupLayout());

    m_canvas = std::make_unique<FKGLCanvas>(this);
    m_canvas->set_background_color({ 255, 255, 255, 255 });
    m_canvas->set_size({ 1024, 800 });

    Window* controls = new Window(this, "Controls");    
    controls->set_position(Vector2i(920, 8));
    controls->set_layout(new GroupLayout());

    Button* resetButton = new Button(controls, "Reset");
    resetButton->set_callback([this]
        {
            reset();
            m_canvas->resetCamera();
        });

    VScrollPanel* scroll = new VScrollPanel(controls);
    scroll->set_fixed_size(Vector2i(360, 800));

    m_links = new Widget(scroll);
    m_links->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 4, 4));

    createLinkSliders(m_armature.get(), m_links, m_linkUIArr);

    Button* simpleButton = new Button(controls, "Simple");
    simpleButton->set_callback([this]
        {
            initializeArmature("resources/simple.bvh");
            createLinkSliders(m_armature.get(), m_links, m_linkUIArr);
            perform_layout();
        });

    Button* characterButton = new Button(controls, "Character");
    characterButton->set_callback([this]
        {
            initializeArmature("resources/character.bvh");
            createLinkSliders(m_armature.get(), m_links, m_linkUIArr);
            perform_layout();
        });

    perform_layout();
    reset();
}

bool FKApplication::keyboard_event(int key, int scancode, int action, int modifiers)
{
    if (Screen::keyboard_event(key, scancode, action, modifiers))
        return true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        set_visible(false);
        return true;
    }
    return false;
}

void FKApplication::draw(NVGcontext* ctx)
{
    assert(m_armature->root != nullptr);

    m_armature->updateKinematics();

    // Draw the user interface
    Screen::draw(ctx);
}

void FKApplication::reset()
{
    // Reset all joints to zero angle
    //
    for (gti320::Link* l : m_armature->links)
    {
        l->eulerAng.setZero();
    }

    // Update the armature
    //
    m_armature->updateKinematics();

    // Update UI
    //
    for (std::unique_ptr<LinkUI>& ui : m_linkUIArr)
    {
        ui->onArmatureChanged();
    }
}

void FKApplication::initializeArmature(const char* bvhFilename)
{
    m_armature = std::make_unique<gti320::Armature>();

    // Initialize the armature
    //
    char errMsg[512];
    BVHData bvh;
    BVHDataInit(&bvh);
    BVHDataLoad(&bvh, bvhFilename, errMsg, 512);
    BVHtoArmature(&bvh, *m_armature);
}
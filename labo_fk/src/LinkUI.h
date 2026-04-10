#pragma once

/**
 * @file LinkUI.h
 *
 * @brief User interface for armature links.
 *
 */

#include <nanogui/window.h>
#include <nanogui/textbox.h>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/slider.h>
#include <nanogui/screen.h>

#include "Math3D.h"

namespace gti320
{
    class Armature;
    class Link;
}

 // Link UI
 //
class LinkUI
{
public:

    LinkUI(gti320::Armature* _armature, gti320::Link* _link, int _id, nanogui::Widget* _parent);

    void onArmatureChanged();

private:

    void init();

    void onSlidersChanged();

    gti320::Armature* armature;
    gti320::Link* link;
    nanogui::ref<nanogui::Widget> panel, panelRotateX, panelRotateY, panelRotateZ;
    nanogui::ref<nanogui::TextBox> textboxRotateX, textboxRotateY, textboxRotateZ;
    nanogui::ref<nanogui::Label> labelRotateX, labelRotateY, labelRotateZ;
    nanogui::ref<nanogui::Slider> sliderRotateX, sliderRotateY, sliderRotateZ;
};

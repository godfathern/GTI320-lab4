#include "LinkUI.h"
#include "Armature.h"

#include <nanogui/button.h>

#include <string>


LinkUI::LinkUI(gti320::Armature* _armature, gti320::Link* _link, int _id, nanogui::Widget* _parent) : armature(_armature), link(_link)
{
	panel = new nanogui::Widget(_parent);
	panel->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Middle, 0, 5));

	init();
}

void LinkUI::init()
{
	static const auto rotateMinMax = std::make_pair<float, float>(-1.57f, 1.57f);

	// Rotate X
	panelRotateX = new nanogui::Widget(panel);
	panelRotateX->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Middle, 0, 20));
	labelRotateX = new nanogui::Label(panelRotateX, link->name + " (X): ");
	sliderRotateX = new nanogui::Slider(panelRotateX);
	sliderRotateX->set_range(rotateMinMax);
	textboxRotateX = new nanogui::TextBox(panelRotateX);
	sliderRotateX->set_callback([this](float value)
		{
			link->eulerAng(0) = value;
			onSlidersChanged();
		});

	// Rotate Y
	panelRotateY = new nanogui::Widget(panel);
	panelRotateY->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Middle, 0, 20));
	labelRotateY = new nanogui::Label(panelRotateY, link->name + " (Y): ");
	sliderRotateY = new nanogui::Slider(panelRotateY);
	sliderRotateY->set_range(rotateMinMax);
	textboxRotateY = new nanogui::TextBox(panelRotateY);
	sliderRotateY->set_callback([this](float value)
		{
			link->eulerAng(1) = value;
			onSlidersChanged();
		});

	// Rotate Z
	panelRotateZ = new nanogui::Widget(panel);
	panelRotateZ->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Middle, 0, 20));
	labelRotateZ = new nanogui::Label(panelRotateZ, link->name + " (Z): ");
	sliderRotateZ = new nanogui::Slider(panelRotateZ);
	sliderRotateZ->set_range(rotateMinMax);
	textboxRotateZ = new nanogui::TextBox(panelRotateZ);
	sliderRotateZ->set_callback([this](float value)
		{
			link->eulerAng(2) = value;
			onSlidersChanged();
		});

	nanogui::Button* button = new nanogui::Button(panel, "Definir racine");
	button->set_callback([this]
		{
			armature->reroot(link);
			onArmatureChanged();
		});

	onSlidersChanged();
}

void LinkUI::onSlidersChanged()
{
	textboxRotateX->set_value(std::to_string(link->eulerAng(0)));
	textboxRotateY->set_value(std::to_string(link->eulerAng(1)));
	textboxRotateZ->set_value(std::to_string(link->eulerAng(2)));
}

void LinkUI::onArmatureChanged()
{
	sliderRotateX->set_value(link->eulerAng(0));
	sliderRotateY->set_value(link->eulerAng(1));
	sliderRotateZ->set_value(link->eulerAng(2));

	onSlidersChanged();
}

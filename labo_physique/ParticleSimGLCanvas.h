#pragma once

/**
 * @file ParticleSimGLCanvas.h
 *
 * @brief Classe Canvas pour l'affichage via OpenGL.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */


#include <nanogui/canvas.h>
#include <nanogui/shader.h>

#include "ParticleSimApplication.h"

/**
 * Classe canvas OpenGL pour afficher des systemes de particules
 */
class ParticleSimGLCanvas : public nanogui::Canvas
{
public:
    ParticleSimGLCanvas(ParticleSimApplication* _app);

    ~ParticleSimGLCanvas();

    virtual void draw_contents() override;

    virtual bool mouse_button_event(const nanogui::Vector2i &p, int button, bool down, int modifiers) override;

    virtual bool mouse_drag_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;

    void applyMouseSpring();

private:

    void convertAndStoreMousePos(const nanogui::Vector2i& mousePos);

    nanogui::ref<nanogui::Shader> m_particleShader;

    ParticleSimApplication* m_app;

    gti320::Particle* m_selectedParticle;
    double m_mouseStiffness;
    gti320::Vector2f m_mousePos;
    gti320::Vector<float> m_circle;

};

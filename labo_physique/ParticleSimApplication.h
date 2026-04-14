#pragma once

/**
 * @file ParticleSimApplication.h
 *
 * @brief Partir applicative du laboratoire 3 : contrôle de la simulaiton via
 * l'interface graphique.
 *
 * Nom: Phan Tung Bui
 * Code permanent : BUIP26109708
 * Email : phan-tung.bui.1@ens.etsmtl.ca
 *
 */


#include <nanogui/window.h>
#include <nanogui/textbox.h>
#include <nanogui/label.h>
#include <nanogui/slider.h>
#include <nanogui/screen.h>

#include "ParticleSystem.h"
#include "GraphColoring.h"
#include "Solvers.h"
#include <chrono>

class ParticleSimGLCanvas;

/**
 * Interface graphique
 */
class ParticleSimApplication : public nanogui::Screen
{
public:
    ParticleSimApplication();

    virtual bool keyboard_event(int key, int scancode, int action, int modifiers) override;

    virtual void draw_contents() override;

    nanogui::Window* getWindow() const { return m_window; }

    const gti320::ParticleSystem& getParticleSystem() const { return m_particleSystem; }

    gti320::ParticleSystem& getParticleSystem() { return m_particleSystem; }

private:

    void initGui();

    /**
     * Effectue un pas de simulation d'un intervale de temps dt.
     */
    void step(float dt);

    /**
     * Fonction appelée lorsque le glisseur de rigidité est modifié
     */
    void onStiffnessSliderChanged();

    /**
     * Réinitialise le système de particules
     */
    void reset();

    void updateFrameCounter();

    ParticleSimGLCanvas* m_canvas;
    nanogui::Window* m_window;

    // Panels
    Widget* m_panelFPS;
    Widget* m_panelFrames;
    Widget* m_panelStiffness;
    Widget* m_panelSolver;
    Widget* m_panelRayleigh;

    // Textboxes 
    nanogui::TextBox* m_textboxFPS;
    nanogui::TextBox* m_textboxFrames;
    nanogui::TextBox* m_textboxStiffness;
    nanogui::TextBox* m_textboxRayleighAlpha;
    nanogui::TextBox* m_textboxRayleighBeta;

    // Labels
    nanogui::Label* m_labelFPS;
    nanogui::Label* m_labelFrames;
    nanogui::Label* m_labelStiffness;
    nanogui::Label* m_labelRayleighAlpha;
    nanogui::Label* m_labelRayleighBeta;

    // Sliders
    nanogui::Slider* m_sliderStiffness;
    nanogui::Slider* m_sliderRayleighAlpha;
    nanogui::Slider* m_sliderRayleighBeta;

    // Le système de particules
    gti320::ParticleSystem m_particleSystem;

    // Coloration de graphe
    gti320::GraphColoring m_graphColor;

    // The variable m_stepping is true if the simulation is running, 
    // and step() will be called automatically
    bool m_stepping;                   // true lorsque la simulation est cours, false lorsqu'elles à l'arrêt
    float m_stiffness;                // la rigidité des ressorts
    int m_maxIter;                     // nombre max d'itération pour les solveurs itératifs
    gti320::eSolverType m_solverType;  // indique le choix du solveur

    // Variables pour le calcul du fps et le compteur de frames
    int m_fpsCounter;
    int m_frameCounter;
    float m_prevTime;

    // Matrices du système
    gti320::Matrix<float, gti320::Dynamic, gti320::Dynamic> m_M;   // matrice de masses
    gti320::Matrix<float, gti320::Dynamic, gti320::Dynamic> m_dfdx;   // matrice de rigidité

    // Vecteurs d'état
    gti320::Vector<float, gti320::Dynamic> m_x;  // positions des particules
    gti320::Vector<float, gti320::Dynamic> m_v;  // vélocités des particules
    gti320::Vector<float, gti320::Dynamic> m_f;  // forces des particules

    // État initial (utilisé pour réinitialiser le système)
    gti320::Vector<float, gti320::Dynamic> m_p0; // positions des particules
    gti320::Vector<float, gti320::Dynamic> m_v0; // vélocités des particules
    gti320::Vector<float, gti320::Dynamic> m_f0; // forces des particules

};

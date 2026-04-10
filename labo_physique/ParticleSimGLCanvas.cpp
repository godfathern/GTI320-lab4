#include "ParticleSimGLCanvas.h"
#include "ParticleSystem.h"

#include <nanogui/opengl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace nanogui;

namespace
{
    static const float r = 6.0f;

    static inline gti320::Particle* pickParticle(std::vector<gti320::Particle>& particles, const gti320::Vector2f& mousePos)
    {
        for (gti320::Particle& particle : particles)
        {
            const float dist = (particle.x - mousePos).norm();
            if (dist <= r)
            {
                return &particle;
            }
        }

        return nullptr;
    }

    static const float fixedColor[3] = { 1.0f, 1.0f, 0.68f };
    static const float defaultColor[3] = { 1.0f, 0.0f, 0.0f };
    static const int maxColors = 8;
    static const int colorMap[maxColors * 3] = {
        228,26,28,
        55,126,184,
        77,175,74,
        152,78,163,
        255,127,0,
        255,255,51,
        166,86,40,
        247,129,191
    };
}

ParticleSimGLCanvas::ParticleSimGLCanvas(ParticleSimApplication* _app) : nanogui::Canvas(_app->getWindow()), m_app(_app), m_selectedParticle(nullptr)
{

    // Un shader minimaliste pour afficher les particules
    m_particleShader = new Shader(render_pass(),
       // Nom du shader
        "particle_shader",
        // Nuanceur de particules
        R"(#version 410
        uniform mat4 modelViewProj;
        uniform vec4 color;
        in vec2 position;
        void main() {
            gl_Position = modelViewProj * vec4(position.xy, -1, 1);
        })",

        // Fragment shader
        R"(#version 410
        uniform vec4 color;
        out vec4 frag_color;
        void main() {
            frag_color = color;
        })"
    );


    // Initialise la géométrie pour un cercle
    static const int numTris = 8;
    m_circle.resize(4 * (numTris+1));
    m_circle.setZero();
    for (int i = 0; i <= numTris; ++i)
    {
        const float angle = 2.0f * M_PI * ((float)i / numTris);
        const float x = r * sin(angle);
        const float y = r * cos(angle);
        m_circle(4 * i) = x;
        m_circle(4 * i + 1) = y;
        m_circle(4 * i + 2) = 0;
        m_circle(4 * i + 3) = 0;
    }

    const Matrix4f mvp = Matrix4f::scale(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_particleShader->set_uniform("modelViewProj", mvp);
    m_particleShader->set_uniform("color", Vector4f(0.0f, 0.0f, 1.0f, 10.0f));
    m_particleShader->set_buffer("position", VariableType::Float32, { (uint32_t)m_circle.rows(), (int)2 }, m_circle.data());
}

ParticleSimGLCanvas::~ParticleSimGLCanvas() {
}

void ParticleSimGLCanvas::draw_contents()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);

    m_particleShader->set_uniform("color", Vector4f(1.0f, 0.0f, 0.0f, 1.0f));

    // Matrice de projection orthographique
    const Matrix4f projMat = Matrix4f::ortho(0, width() - 1, 0, height() - 1, 0.1f, 1.0f);
    const gti320::ParticleSystem& particleSystem = m_app->getParticleSystem();
    const auto particles = particleSystem.getParticles();
    const int numParticles = particles.size();

    // Affichage des ressorts
    const auto springs = particleSystem.getSprings();
    const int numSprings = springs.size();
    std::vector<float> points(4 * numSprings);
    for (int i = 0; i < numSprings; ++i)
    {
        const int index0 = springs[i].index0;
        const int index1 = springs[i].index1;
        const int k = 4 * i;
        memcpy(&points[k], particles[index0].x.data(), sizeof(float) * 2);
        memcpy(&points[k+2], particles[index1].x.data(), sizeof(float) * 2);
    }
    m_particleShader->set_uniform("modelViewProj", projMat);
    m_particleShader->set_uniform("color", Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
    m_particleShader->set_buffer("position", VariableType::Float32, { (uint32_t)points.size() / 2, 2 }, points.data());

    m_particleShader->begin();
    m_particleShader->draw_array(Shader::PrimitiveType::Line, 0, points.size() / 2);
    m_particleShader->end();

    // Affichage des particules   
    m_particleShader->set_buffer("position", VariableType::Float32, { (uint32_t)m_circle.rows() / 2, 2 }, m_circle.data());
    for (int i = 0; i < numParticles; ++i)
    {
        Matrix4f modelMat = Matrix4f::scale(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
        modelMat.m[3][0] = particles[i].x(0);
        modelMat.m[3][1] = particles[i].x(1);
        const Matrix4f mvp = projMat * modelMat;
        m_particleShader->set_uniform("modelViewProj", mvp);

        if (particles[i].color > -1)
        {
            const int c = std::min(particles[i].color, maxColors - 1);
            const int i = 3 * c;
            m_particleShader->set_uniform("color", Vector4f((float)colorMap[i]/255.0f, (float)colorMap[i+1] / 255.0f, (float)colorMap[i+2] / 255.0f, 1.0f));
        }
        else
        {
            m_particleShader->set_uniform("color", Vector4f(defaultColor[0], defaultColor[1], defaultColor[2], 1.0f));
        }

        m_particleShader->begin();
        m_particleShader->draw_array(Shader::PrimitiveType::TriangleStrip, 0, m_circle.rows() / 2);
        m_particleShader->end();
    }

    // Affichage du ressort déféni par la souris
    if (m_selectedParticle)
    {
        const gti320::Vector2f p = m_selectedParticle->x;
        const float coords[4] = { m_mousePos(0), m_mousePos(1), p(0), p(1) };
        m_particleShader->set_uniform("modelViewProj", projMat);
        m_particleShader->set_uniform("color", Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
        m_particleShader->set_buffer("position", VariableType::Float32, { 2, 2 }, coords);

        m_particleShader->begin();
        m_particleShader->draw_array(Shader::PrimitiveType::Line, 0, 2);
        m_particleShader->end();
    }

}

bool ParticleSimGLCanvas::mouse_button_event(const Vector2i& p, int button, bool down, int modifiers)
{
    if (modifiers == GLFW_MOD_SHIFT)
    {
        if (button == GLFW_MOUSE_BUTTON_1 && down)
        {
            convertAndStoreMousePos(p);
            m_selectedParticle = pickParticle(m_app->getParticleSystem().getParticles(), m_mousePos);
            if (m_selectedParticle != nullptr)
            {
                m_selectedParticle->fixed = !(m_selectedParticle->fixed);
                m_selectedParticle = nullptr;
            }
            return true;
        }
    }
    else
    {
        if (button == GLFW_MOUSE_BUTTON_1 && down)
        {
            convertAndStoreMousePos(p);
            m_selectedParticle = pickParticle(m_app->getParticleSystem().getParticles(), m_mousePos);
            return true;
        }
        else if (button == 0)
        {
            m_selectedParticle = nullptr;
            return true;
        }
    }

    return false;
}

bool ParticleSimGLCanvas::mouse_drag_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers)
{
    if (button == GLFW_MOUSE_BUTTON_2 && modifiers == 0 && m_selectedParticle != nullptr)
    {
        convertAndStoreMousePos(p);
        return true;
    }

    return false;
}

void ParticleSimGLCanvas::convertAndStoreMousePos(const Vector2i& mousePos)
{
    const Vector2i& pos = position();
    const int y = height() - (mousePos.y() - pos.y()) - 1;
    m_mousePos(0) = (float)(mousePos.x() - pos.x());
    m_mousePos(1) = (float)y;
}

void ParticleSimGLCanvas::applyMouseSpring()
{
    if (m_selectedParticle != nullptr)
    {
        const float k = 20.0f * m_selectedParticle->m;
        const gti320::Vector2f f = k * (m_mousePos - m_selectedParticle->x);
        m_selectedParticle->f = m_selectedParticle->f + f;
    }
}

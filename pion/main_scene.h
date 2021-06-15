#pragma once

#include "scene.h"

#include "camera.h"
#include "skybox.h"
#include "light.h"

#include "particle_system.h"

class main_window;

class main_scene : public scene
{
public:
    main_scene(main_window& wnd);

    virtual void update(float dt) override;
    virtual void draw() override;

    virtual camera& get_camera() override;
    virtual skybox& get_skybox() override;
    virtual light& get_light() override;

private:
    main_window& m_wnd;

    camera m_camera;
    skybox m_skybox;
    light m_light;

    particle_system m_ps;

    float m_elapsedTime = 0.0f;
    int m_tick = 0;
};

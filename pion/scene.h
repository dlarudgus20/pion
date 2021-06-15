#pragma once

class camera;
class skybox;
struct light;

class scene
{
public:
    scene(const scene&) = delete;
    scene& operator =(const scene&) = delete;

    scene() = default;
    virtual ~scene() = default;

    virtual void update(float dt) = 0;
    virtual void draw() = 0;

    virtual camera& get_camera() = 0;
    virtual skybox& get_skybox() = 0;
    virtual light& get_light() = 0;
};

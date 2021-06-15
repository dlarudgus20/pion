#pragma once

#include "shader.h"
#include "material.h"

#include "particle_system.cuh"

class scene;

using ps_cuda::particle;

class particle_system
{
public:
    particle_system(const particle_system&) = delete;
    particle_system& operator =(const particle_system&) = delete;

    particle_system();
    ~particle_system();

    void draw(scene& sc);

    void add_particle_bunch(float dt);
    void add_particle(int n);
    void remove_particle(int n);

    void update(float dt);

    int particle_count() const
    {
        return m_particles.size();
    }

private:
    particle generate();

    GLuint m_vbo, m_vao;
    shader m_shader;

    material m_mat;
    float m_time = 0;

    std::vector<particle> m_particles;

    std::mt19937 m_rd;

    float m_remain_time = 0.0f;
};

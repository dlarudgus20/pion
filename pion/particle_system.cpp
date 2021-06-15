#include "pch.h"
#include "particle_system.h"
#include "scene.h"
#include "camera.h"
#include "skybox.h"
#include "light.h"

namespace
{
    constexpr int MAX_PARTICLES = 100000;
    constexpr int ADD_PARTICLES = 4000;

    constexpr float PARTICLE_SIZE = 0.02f;

    const GLfloat vertices[] = {
        -1, -1,
        1, -1,
        1, 1,
        -1, -1,
        1, 1,
        -1, 1,
    };
    constexpr GLsizei strides = 2 * sizeof(GLfloat);
}

particle_system::particle_system()
    : m_shader("shaders/particle.vert", "shaders/particle.frag")
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strides, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    m_mat.ambient = glm::vec4(0.5);
    m_mat.diffuse = glm::vec4(0.0);
    m_mat.specular = glm::vec4(1.0);
    m_mat.shininess = 128.0f;
    m_mat.refr_index = 0.65;
    m_mat.refl_cof = 0.8;
    m_mat.refr_cof = 0.2;

    std::random_device rd;
    m_rd.seed(rd());

    ps_cuda::initialize();
}

particle_system::~particle_system()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void particle_system::draw(scene& sc)
{
    auto& projection = sc.get_camera().get_projection();
    auto& view = sc.get_camera().get_view();

    glm::mat4 model;

    m_shader.use();
    m_shader.setUniform4fm("projection", projection);
    m_shader.setUniform4fm("view_inv", glm::inverse(view));
    m_shader.setUniform3f("camera_pos", sc.get_camera().get_pos());
    sc.get_light().apply(m_shader);
    m_mat.apply(m_shader);
    for (auto& particle : m_particles)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, particle.position);
        model = glm::scale(model, glm::vec3(PARTICLE_SIZE));
        glm::mat4 view_model = view * model;
        m_shader.setUniform4fm("view_model", view_model);
        m_shader.setUniform3fm("matN", glm::mat3(glm::transpose(glm::inverse(model))));

        glDisable(GL_CULL_FACE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, sc.get_skybox().get_cube_map());
        m_shader.setUniform1i("skybox", 0);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / strides);
        glBindVertexArray(0);
    }
}

void particle_system::add_particle_bunch(float dt)
{
    dt += m_remain_time;
    int n = int(ADD_PARTICLES * dt);
    dt -= n * 1.0f / ADD_PARTICLES;

    add_particle(n);
    m_remain_time = dt;
}

void particle_system::add_particle(int n)
{
    if (n > MAX_PARTICLES)
        n = MAX_PARTICLES;

    remove_particle(m_particles.size() + n - MAX_PARTICLES);

    for (int i = 0; i < n; ++i)
    {
        m_particles.push_back(generate());
    }
}

particle particle_system::generate()
{
    std::uniform_real_distribution<float> vxdist(-0.2f, 0.2f);
    std::uniform_real_distribution<float> vydist(0.7f, 1.4f);
    std::uniform_real_distribution<float> vzdist(-0.2f, 0.2f);

    particle p;
    p.mass = 1;
    p.position = glm::vec3(0.0f, -1.0f, 0.0f);
    p.velocity = glm::vec3(vxdist(m_rd), vydist(m_rd), vzdist(m_rd));

    return p;
}

void particle_system::remove_particle(int n)
{
    if (n < 0)
        return;
    if (n > m_particles.size())
        n = m_particles.size();

    m_particles.erase(m_particles.begin(), m_particles.begin() + n);
}

void particle_system::update(float dt)
{
    m_time += dt;

    m_mat.ambient = glm::vec4(0.5f + 0.5f * cosf(m_time));
    m_mat.diffuse = glm::vec4(0.5f - 0.5f * cosf(m_time));
    m_mat.specular = glm::vec4(1.0);

    ps_cuda::particle_update(m_particles, dt);
}

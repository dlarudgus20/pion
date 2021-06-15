#pragma once

#ifdef __CUDACC__
#define GLM_FORCE_CUDA
#define alignas(n) __align__(n)
#endif

#include <glm/glm.hpp>

#include <vector>

namespace ps_cuda
{
    struct particle
    {
        float mass;
        float radius;
        glm::vec3 position;
        glm::vec3 velocity;
    };

    struct image
    {
        unsigned char* data;
        int width, height;
    };

    struct cubemap
    {
        image images[6];
    };

    void initialize();

    void particle_update(std::vector<particle>& particles, float dt);
}

#include <GL/glew.h>

#include <cuda.h>

#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

#include <stdexcept>

#include "particle_system.cuh"

using namespace ps_cuda;

__global__ void particle_update_kernel(particle* particles, int num_particles, float dt)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= num_particles)
        return;

    auto& particle = particles[idx];

    glm::vec3 force = glm::vec3(0, -0.2f * particle.mass, 0);

    glm::vec3 a = force / particle.mass;
    glm::vec3 dv = a * dt;

    particle.position += (particle.velocity + dv * 0.5f) * dt;
    particle.velocity += dv;

    float y = particle.position.y - particle.radius + 1.1f;
    if (y < 0.0f)
    {
        particle.position.y = -y + particle.radius - 1.1f;
        particle.velocity.y *= -0.3f;
    }
}

void ps_cuda::initialize()
{
    if (cudaSetDevice(0) != cudaSuccess)
        throw std::runtime_error("failed to initialize cuda");
}

void ps_cuda::particle_update(std::vector<particle>& particles, float dt)
{
    constexpr int block = 128;
    int grid = (particles.size() + block - 1) / block;

    particle* device_particles;
    cudaMalloc((void**)&device_particles, sizeof(particle) * particles.size());
    cudaMemcpy(device_particles, particles.data(), sizeof(particle) * particles.size(), cudaMemcpyHostToDevice);

    particle_update_kernel<<<grid, block>>>(device_particles, particles.size(), dt);

    cudaMemcpy(particles.data(), device_particles, sizeof(particle) * particles.size(), cudaMemcpyDeviceToHost);
    cudaFree(device_particles);
}

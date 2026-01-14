#pragma once

#include "cmbst_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>
namespace cmbst
{
  class CmbstModel
  {
    public:

      struct Vertex
      {
        glm::vec2 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

      };

      CmbstModel(CmbstDevice &device, const std::vector<Vertex> &vertices);
      ~CmbstModel();

      CmbstModel(const CmbstWindow &) = delete;
      CmbstModel &operator=(const CmbstWindow &) = delete;

      void bind(VkCommandBuffer commandBuffer);
      void draw(VkCommandBuffer commandBuffer);
    private:
      void createVertexBuffers(const std::vector<Vertex> &vertices);

      CmbstDevice& cmbstDevice;
      VkBuffer vertexBuffer;
      VkDeviceMemory vertexBufferMemory;
      uint32_t vertexCount;
  };
} // namespace cmbst

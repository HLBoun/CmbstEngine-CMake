
#pragma once


#include "cmbst_pipeline.hpp"
#include "cmbst_device.hpp"
#include "cmbst_game_object.hpp"

// std
#include <memory>
#include <vector>


namespace cmbst 
{
  class SimpleRenderSystem
  {
    public:    
      SimpleRenderSystem(CmbstDevice &device, VkRenderPass renderPass);
      ~SimpleRenderSystem();

      SimpleRenderSystem(const CmbstWindow &) = delete;
      SimpleRenderSystem &operator=(const CmbstWindow &) = delete;

      void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<CmbstGameObject> &gameObjects);

    private:
      void createPipelineLayout();
      void createPipeline(VkRenderPass renderPass);

      CmbstDevice& cmbstDevice;

      std::unique_ptr<CmbstPipeline> cmbstPipeline;
      VkPipelineLayout pipelineLayout;
  };

} // namespace cmbst

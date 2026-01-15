#pragma once


#include "cmbst_window.hpp"
#include "cmbst_pipeline.hpp"
#include "cmbst_device.hpp"
#include "cmbst_game_object.hpp"
#include "cmbst_renderer.hpp"

// std
#include <memory>
#include <vector>


namespace cmbst 
{
  class FirstApp
  {
    public:
      static constexpr int WIDTH  = 800;
      static constexpr int HEIGHT = 600;
      
      FirstApp();
      ~FirstApp();

      FirstApp(const CmbstWindow &) = delete;
      FirstApp &operator=(const CmbstWindow &) = delete;

      void run();
    
    private:
      void loadGameObjects();
      void createPipelineLayout();
      void createPipeline();
      void renderGameObjects(VkCommandBuffer commandBuffer);

      CmbstWindow cmbstWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      CmbstDevice cmbstDevice {cmbstWindow};
      CmbstRenderer cmbstRenderer{ cmbstWindow, cmbstDevice };

      std::unique_ptr<CmbstPipeline> cmbstPipeline;
      VkPipelineLayout pipelineLayout;
      std::vector<CmbstGameObject> gameObjects;
  };

} // namespace cmbst

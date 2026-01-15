#pragma once


#include "cmbst_window.hpp"
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

      CmbstWindow cmbstWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
      CmbstDevice cmbstDevice {cmbstWindow};
      CmbstRenderer cmbstRenderer{ cmbstWindow, cmbstDevice };

      std::vector<CmbstGameObject> gameObjects;
  };

} // namespace cmbst

#include "first_app.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


// std
#include <stdexcept>
#include <cassert>
#include <array>


namespace cmbst
{
  FirstApp::FirstApp()
  {
    loadGameObjects();
  }

  FirstApp::~FirstApp()
  {

  }

  void FirstApp::loadGameObjects()
  {
    std::vector<CmbstModel::Vertex> vertices
    {
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    auto cmbstModel = std::make_shared<CmbstModel>(cmbstDevice, vertices);

    auto triangle = CmbstGameObject::createGameObject();
    triangle.model = cmbstModel;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, .5f};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
  }

  void FirstApp::run()
  {
    SimpleRenderSystem simpleRenderSystem{cmbstDevice, cmbstRenderer.getSwapChainRenderPass()};

    while (!cmbstWindow.shouldClose())
    {
      glfwPollEvents();
      if (auto commandBuffer = cmbstRenderer.beginFrame())
      {
	  cmbstRenderer.beginSwapChainRenderPass(commandBuffer);
	  simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
	  cmbstRenderer.endSwapChainRenderPass(commandBuffer);
	  cmbstRenderer.endFrame();
      }
    }
  
    vkDeviceWaitIdle(cmbstDevice.device());
  }


} // namespace cmbst

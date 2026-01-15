#include "cmbst_renderer.hpp"


// std
#include <stdexcept>
#include <cassert>
#include <array>


namespace cmbst
{
  CmbstRenderer::CmbstRenderer(CmbstWindow &window, CmbstDevice &device) : cmbstWindow{window}, cmbstDevice{device}
  {
    recreateSwapChain();
    createCommandBuffers();
  }

  CmbstRenderer::~CmbstRenderer()  { freeCommandBuffers(); }


  void CmbstRenderer::recreateSwapChain()
  {
      auto extent = cmbstWindow.getExtent();
      while (extent.width == 0 || extent.height == 0)
      {
          extent = cmbstWindow.getExtent();
          glfwWaitEvents();
      }
      vkDeviceWaitIdle(cmbstDevice.device());

      if (cmbstSwapChain == nullptr)
      {
          cmbstSwapChain = std::make_unique<CmbstSwapChain>(cmbstDevice, extent);
      }
      else
      {
	  std::shared_ptr<CmbstSwapChain> oldSwapChain = std::move( cmbstSwapChain);
          cmbstSwapChain = std::make_unique<CmbstSwapChain>(cmbstDevice, extent, oldSwapChain);

	  if(!oldSwapChain->compareSwapFormats(*cmbstSwapChain.get()))
	  {
	      throw std::runtime_error("Swap chain image(or depth) format has changed!");
	  }

      }
      // we'll come back to this
  }
  

  void CmbstRenderer::createCommandBuffers()
  {
    commandBuffers.resize(CmbstSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmbstDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(cmbstDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate command buffers!");
    }
  }

  void CmbstRenderer::freeCommandBuffers()
  {
      vkFreeCommandBuffers
      (
          cmbstDevice.device(),
          cmbstDevice.getCommandPool(),
          static_cast<uint32_t>(commandBuffers.size()),
          commandBuffers.data()
      );
      commandBuffers.clear();
  }


  VkCommandBuffer CmbstRenderer::beginFrame()
  {
    assert(!isFrameStarted && "Can't call beginFrame while already in progress");

    auto result = cmbstSwapChain->acquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
	recreateSwapChain();
	return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
	throw std::runtime_error("failed to acquire swap chain image!");
    }
    
    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to begin recording command buffer!");
    }
    return commandBuffer;
  }
  void CmbstRenderer::endFrame()
  {
      assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      auto commandBuffer = getCurrentCommandBuffer();

      if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
      {
	throw std::runtime_error("failed to record command buffer!");
      }

      auto result = cmbstSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || cmbstWindow.wasWindowResized())
      {
        cmbstWindow.resetWindowResizedFlag();
        recreateSwapChain();
      }
      else if(result != VK_SUCCESS)
      {
        throw std::runtime_error("failed to present swap chain image!");
      }

      isFrameStarted = false;
      currentFrameIndex = (currentFrameIndex + 1) % CmbstSwapChain::MAX_FRAMES_IN_FLIGHT;
  }

  void CmbstRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
  {
      assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");
    
      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = cmbstSwapChain->getRenderPass();
      renderPassInfo.framebuffer = cmbstSwapChain->getFrameBuffer(currentImageIndex);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = cmbstSwapChain->getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = static_cast<float>(cmbstSwapChain->getSwapChainExtent().width);
      viewport.height = static_cast<float>(cmbstSwapChain->getSwapChainExtent().height);
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      VkRect2D scissor{ {0, 0}, cmbstSwapChain->getSwapChainExtent() };
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  }
  void CmbstRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
  {

      assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

      vkCmdEndRenderPass(commandBuffer);
  }
} // namespace cmbst

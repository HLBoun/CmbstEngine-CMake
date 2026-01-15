
#pragma once


#include "cmbst_window.hpp"
#include "cmbst_device.hpp"
#include "cmbst_swap_chain.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>


namespace cmbst 
{
  class CmbstRenderer
  {
    public:
      CmbstRenderer(CmbstWindow &window, CmbstDevice &device);
      ~CmbstRenderer();

      CmbstRenderer(const CmbstWindow &) = delete;
      CmbstRenderer &operator=(const CmbstWindow &) = delete;

      VkRenderPass getSwapChainRenderPass() const { return  cmbstSwapChain->getRenderPass(); }
      bool isFrameInProgress() const { return isFrameStarted; }

      VkCommandBuffer getCurrentCommandBuffer() const
      {
	  assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
	  return commandBuffers[currentImageIndex]; 
      }

      VkCommandBuffer beginFrame();
      void endFrame();
      void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
      void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	
    
    private:
      void createCommandBuffers();
      void freeCommandBuffers();
      void recreateSwapChain();

      CmbstWindow& cmbstWindow;
      CmbstDevice& cmbstDevice;
      std::unique_ptr<CmbstSwapChain> cmbstSwapChain;
      std::vector<VkCommandBuffer> commandBuffers;

      uint32_t currentImageIndex;
      bool isFrameStarted = false;

  };

} // namespace cmbst

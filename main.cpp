#include <array>
#include <iostream>

#include "include/GLFW/glfw3.h"
#include <vulkan/vulkan.h>

#include "app/VulkanApp.h"

int main()
{
  std::unique_ptr<VulkanApp> app;

  try
  {
    app = std::make_unique<VulkanApp>();
    app->run();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << '\n';
    return -1;
  }
}

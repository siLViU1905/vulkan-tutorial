#include <array>
#include <iostream>
#include <thread>

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

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    return -1;
  }
}

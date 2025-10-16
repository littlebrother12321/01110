#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vulkan/vulkan.h> //For my computer I needed to import this... so I added this (Dave)
#include <vector>

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(800, 600, "01110 epic window of epiccness TOO!", NULL, NULL); //Would this edited name be more accurate? (Dave)
  if (window == NULL)
    {
      std::cout << "uh oh window couldn't, oopsie. I am crash now." << std::endl;
      glfwTerminate();
      return -1;
    };
  glfwMakeContextCurrent(window);
  
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); //Shortest vulkan name of thing ever: vkEnumerateInstanceExtensionProperties. Lol. (Dave)

  std::cout << extensionCount << " extensions supported\n"; //I get 24 (dave)

  // Print all the extensions you have but like their names too
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()); //Shortest vulkan name of thing ever: vkEnumerateInstanceExtensionProperties. Lol. (Dave)
  
  for (const auto& ext : extensions) {
    std::cout << "\t" << ext.extensionName << "\n";
  }

  glm::mat4 matrix;
  glm::vec4 vec;
  auto test = matrix * vec;

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;

}

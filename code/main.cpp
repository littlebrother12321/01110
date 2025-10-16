#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vulkan/vulkan.h> //For my computer I needed to import this... so I added this (Dave)
#include <vector>
//I looked into the toto-rial and its not acctually mentioneds it ment to have acctually works... 
//so yeah I think we finished this part of the totorial... probably
//I think we should get started on the next part probably probably (Probably dave)
int main() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND); //added this line so it might acctually make a window I can see, it didn't work (dave)
  GLFWwindow* window = glfwCreateWindow(800, 600, "01110 epic window of epiccness TOO!", NULL, NULL); //Would this edited name be more accurate? (Dave) // OK. (luke)
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
                                                            //ditto (luke)

  // Print all the extensions you have but like their names too
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()); //Shortest vulkan name of thing ever: vkEnumerateInstanceExtensionProperties. Lol. (Dave)
  
  for (const auto& ext : extensions) {
    std::cout << "\t" << ext.extensionName << "\n";
  }

  //trying some random stuff the nasty AI told me to do (luke) //I commented out cus its thorwing errors at me : (
  // VkSurfaceKHR* surface;
  // if (glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS) { // May need to define an instance (luke)
  //   throw std::runtime_error("no window surface successed");
  // }

  // May need to build GLFW from source with wayland support on (luke)
  
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

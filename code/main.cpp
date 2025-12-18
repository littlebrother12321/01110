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

void glfw_error_callback(int code, const char* desc) {
  std::cerr << "GLFW error " << code << ": " << desc << "\n";
}

int main() {

  if (!glfwInit()) {
    const char* desc;
    int code = glfwGetError(&desc);
    std::cerr << "GLFW init fialed: " << (desc ? desc : "unknown") << "\n";
    return -1;
  }

  int platform = glfwGetPlatform();
  switch (platform) {
  case GLFW_PLATFORM_X11:
    std::cout << "GLFW is currently on X11\n";
    break;
  case GLFW_PLATFORM_WAYLAND:
    std::cout << "GLFW is currently on Wayland\n";
    break;
  default:
    std::cout << "GLFW is currently on another\n";
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(800, 600, "01110 epic window of epiccness TOO!", NULL, NULL); //Would this edited name be more accurate? (Dave) // OK. (luke)
  if (window == NULL)
    {
      std::cout << "uh oh window couldn't, oopsie. I am crash now." << std::endl;
      glfwTerminate();
      return -1;
    };
  //glfwMakeContextCurrent(window);
  glfwShowWindow(window);


  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions =
    glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::cout << "These are the extensions that GLFW requires:\n";
  for (uint32_t i = 0; i < glfwExtensionCount; i++) {
    std::cout << "  " << glfwExtensions[i] << "\n";
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Epic Window";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create Vulkan instance");
  }

  
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

  VkSurfaceKHR surface;
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }

  // Get physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0)
    throw std::runtime_error("no Vulkan devices");

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  VkPhysicalDevice physicalDevice = devices[0];

  // Find graphics + present queue families
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(
					   physicalDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
					   physicalDevice, &queueFamilyCount, queueFamilies.data());

  int graphicsFamily = -1;
  int presentFamily = -1;

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      graphicsFamily = i;

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
					 physicalDevice, i, surface, &presentSupport);

    if (presentSupport)
      presentFamily = i;
  }

  if (graphicsFamily == -1 || presentFamily == -1)
    throw std::runtime_error("required queue families not found");

  // Create logical device + queues
  float priority = 1.0f;

  std::vector<VkDeviceQueueCreateInfo> queues;
  std::vector<uint32_t> uniqueFamilies = {
    (uint32_t)graphicsFamily,
    (uint32_t)presentFamily
  };

  for (uint32_t fam : uniqueFamilies) {
    VkDeviceQueueCreateInfo q{};
    q.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    q.queueFamilyIndex = fam;
    q.queueCount = 1;
    q.pQueuePriorities = &priority;
    queues.push_back(q);
  }

  const char* deviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  VkDeviceCreateInfo dci{};
  dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  dci.queueCreateInfoCount = queues.size();
  dci.pQueueCreateInfos = queues.data();
  dci.enabledExtensionCount = 1;
  dci.ppEnabledExtensionNames = deviceExtensions;

  VkDevice device;
  vkCreateDevice(physicalDevice, &dci, nullptr, &device);

  VkQueue graphicsQueue;
  VkQueue presentQueue;
  vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
  vkGetDeviceQueue(device, presentFamily, 0, &presentQueue);


  
  // Create swapchain
  VkSurfaceCapabilitiesKHR caps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
					    physicalDevice, surface, &caps);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
				       physicalDevice, surface, &formatCount, nullptr);

  std::vector<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(
				       physicalDevice, surface, &formatCount, formats.data());

  VkSurfaceFormatKHR format = formats[0];

  VkSwapchainCreateInfoKHR sci{};
  sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  sci.surface = surface;
  uint32_t imageCount = caps.minImageCount + 1;
  if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount) {
    imageCount = caps.maxImageCount;
  }
  sci.minImageCount = imageCount;
  sci.imageFormat = format.format;
  sci.imageColorSpace = format.colorSpace;
  sci.imageExtent = caps.currentExtent;
  sci.imageArrayLayers = 1;
  sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  sci.preTransform = caps.currentTransform;
  sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  sci.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  sci.clipped = VK_TRUE;

  VkSwapchainKHR swapchain;
  vkCreateSwapchainKHR(device, &sci, nullptr, &swapchain);

  // Acquire image
  uint32_t imageIndex;
  vkAcquireNextImageKHR(
			device,
			swapchain,
			UINT64_MAX,
			VK_NULL_HANDLE,
			VK_NULL_HANDLE,
			&imageIndex
			);

  // Present image
  VkPresentInfoKHR pi{};
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.swapchainCount = 1;
  pi.pSwapchains = &swapchain;
  pi.pImageIndices = &imageIndex;

  vkQueuePresentKHR(presentQueue, &pi);
  vkQueueWaitIdle(presentQueue);

  //!!! All of this junk was made by nasty AI, although it somewhat does work. i got the (MESA-INTEL: warning: Ivy Bridge Vulkan support is incomplete) message, which i forgot about 'till now.
  // Conclusion: I don't like vulkan, too much effort for one window.
  
  glm::mat4 matrix;
  glm::vec4 vec;
  auto test = matrix * vec;

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  vkDeviceWaitIdle(device);
  vkDestroySwapchainKHR(device, swapchain, nullptr);
  vkDestroyDevice(device, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyInstance(instance, nullptr);

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;

}

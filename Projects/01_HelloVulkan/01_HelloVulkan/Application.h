#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

class Application
{
public: 
    void run();

private:
    GLFWwindow* window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef _DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanUp();

    void createInstance();

    bool checkValidationLayerSupport();
    void checkExtensionSupport();
    std::vector<const char*> getRequiredExtensions();
    void setupDebugMessenger();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
};
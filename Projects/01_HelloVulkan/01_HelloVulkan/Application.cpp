#include "Application.h"

#include <vector>


void Application::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}

void Application::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void Application::initVulkan()
{
    checkExtensionSupport();
    createInstance();
}

void Application::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void Application::cleanUp()
{
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    //Defining VkApplicaitonInfo is optional,
    //but it is good to provide useful info to the driver in order to optimize our specific application.
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;


    //This is must.
    //VkInstanceCreateInfo tells the vulkan driver which global extensions and validation layers we want to use.
    //Global means, it applies to the entire program. not a specific device.
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;


    /*
     * Vulkan is a platform agnostic API
     * Which means, you need an extension to interface with the window system
     */

    /*
     * These codes can be replaced with codes right below
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    */
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();


    //enableValidationLayers is set to true in debug mode to enable validation layers.
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    //Nearly all vulkan functions return a value of type VkResult.
    //That is either VK_SUCCESS or error code.
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }


}

bool Application::checkValidationLayerSupport()
{
    //Vulkan API is designed around the idea of minimal driver overhead
    //Therefore, it has very limited error checking in the API by default
    //But, Vulkan has validation layers which hook into vulkan function calls to apply additional operations.
    //Validation layers can only be used if they have been installed onto system.
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void Application::checkExtensionSupport()
{
    //print out which extensions are supported
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "available extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

std::vector<const char*> Application::getRequiredExtensions()
{
    //This function will return the required list of extensions
    // based on whether validation layers are enabled or not
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); //VK_EXT_DEBUG_UTILS_EXTENSION_NAME is equal to literal string "VK_EXT_debug_utils"
    }

    return extensions;
}

void Application::setupDebugMessenger()
{
    //TODO: setup debugmessenger
    if (!enableValidationLayers)
        return;

  
}

VkBool32 Application::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                    VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                    void* pUserData)
{
    /*
     * messageSeverity parameter can have the following values:
     * VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
     * VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
     * VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
     * VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes
     */

    /*
     * messageType parameter can have the following values:
     * VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
     * VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
     * VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan
     */

    /*
     * pCallbackData parameter refers to a VkDebugUtilsMessengerCallbackDataEXT struct containing the details of the message itself, with the most important members being:
     * pMessage: The debug message as a null-terminated string
     * pObjects: Array of Vulkan object handles related to the message
     * objectCount: Number of objects in array
     */

    /*
     * pUserData parameter contains a pointer that was specified during the setup of the callback and allows you to pass your own data to it.
     */



    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    /*
     * The callback returns a boolean that indicates if the Vulkan call that triggered the validation layer message should be aborted.
     * If the callback returns true, then the call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error.
     * This is normally only used to test the validation layers themselves, so you should always return VK_FALSE.
     */
    return VK_FALSE;
}

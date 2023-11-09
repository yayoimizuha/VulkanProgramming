#include <iostream>
#include <vulkan/vulkan.hpp>

using namespace std;
using namespace vk;

int main() {
    InstanceCreateInfo instanceCreateInfo;
    UniqueInstance instance = createInstanceUnique(instanceCreateInfo);
    auto physicalDevices = instance->enumeratePhysicalDevices();
    auto physicalDevice = physicalDevices.at(0);


    auto requiredLayers = {"VK_LAYER_KHRONOS_validation"};

    DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.enabledLayerCount = requiredLayers.size();
    deviceCreateInfo.ppEnabledLayerNames = requiredLayers.begin();

    auto device = physicalDevice.createDeviceUnique(deviceCreateInfo);

    auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    for (auto queueFamilyProperty: queueFamilyProperties) {
        cout << "Queue count: " << queueFamilyProperty.queueCount << endl;
        cout << "Graphics Queue support : " << boolalpha
             << (bool) (queueFamilyProperty.queueFlags & QueueFlagBits::eGraphics)
             << endl;
        cout << "Compute Queue support : " << boolalpha
             << (bool) (queueFamilyProperty.queueFlags & QueueFlagBits::eCompute)
             << endl;
        cout << "Transfer Queue support : " << boolalpha
             << (bool) (queueFamilyProperty.queueFlags & QueueFlagBits::eTransfer)
             << endl;
        cout << endl;


    }


    return 0;
}

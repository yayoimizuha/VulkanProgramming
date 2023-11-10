#include <iostream>
#include <vulkan/vulkan.hpp>

using namespace std;
using namespace vk;

int main() {
    InstanceCreateInfo instanceCreateInfo;
    UniqueInstance instance = createInstanceUnique(instanceCreateInfo);
    auto physicalDevices = instance->enumeratePhysicalDevices();
//    auto physicalDevice = physicalDevices.at(0);
    PhysicalDevice suitableDevice;
    size_t queueIndex;

    for (auto &device: physicalDevices) {
        cout << device.getProperties().deviceName << endl;
        auto queueFamilyProperties = device.getQueueFamilyProperties();
        for (int i = 0; i < queueFamilyProperties.size(); ++i) {
            auto queueFamilyProperty = &queueFamilyProperties.at(i);
            cout << "\t" << "Queue index: " << i << endl;
            cout << "\t" << "Queue count: " << queueFamilyProperty->queueCount << endl;
            cout << "\t" << "Graphics Queue support : " << boolalpha
                 << (bool) (queueFamilyProperty->queueFlags & QueueFlagBits::eGraphics)
                 << endl;
            cout << "\t" << "Compute Queue support : " << boolalpha
                 << (bool) (queueFamilyProperty->queueFlags & QueueFlagBits::eCompute)
                 << endl;
            cout << "\t" << "Transfer Queue support : " << boolalpha
                 << (bool) (queueFamilyProperty->queueFlags & QueueFlagBits::eTransfer)
                 << endl;
            cout << endl;
            if (queueFamilyProperty->queueFlags & QueueFlagBits::eGraphics) {
                suitableDevice = device;
                queueIndex = i;
            }
        }
    }
    if (suitableDevice == (PhysicalDevice)nullptr){
        cout << "Not found suitable device." << endl;
        exit(-1);
    }


    DeviceQueueCreateInfo deviceQueueCreateInfo;
    deviceQueueCreateInfo.queueFamilyIndex = queueIndex;
    deviceQueueCreateInfo.queueCount = 1;
    float queuePriorities[1] = {1.0f};
    deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

    DeviceCreateInfo deviceCreateInfo;
    auto requiredLayers = {"VK_LAYER_KHRONOS_validation"};
    deviceCreateInfo.enabledLayerCount = requiredLayers.size();
    deviceCreateInfo.ppEnabledLayerNames = requiredLayers.begin();
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

    auto device = suitableDevice.createDeviceUnique(deviceCreateInfo);
    auto queue = device->getQueue(queueIndex, 0);


    CommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.queueFamilyIndex = queueIndex;
    auto commandPool = device->createCommandPoolUnique(commandPoolCreateInfo);


    CommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.commandPool = commandPool.get();
    commandBufferAllocateInfo.commandBufferCount = 1;
    commandBufferAllocateInfo.level = CommandBufferLevel::ePrimary;
    auto commandBuffers = device->allocateCommandBuffersUnique(commandBufferAllocateInfo);

    const uint32_t imageHeight = 2160;
    const uint32_t imageWidth = 3840;
    ImageCreateInfo imageCreateInfo;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.extent = Extent3D(imageWidth, imageHeight, 1);
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = Format::eR8G8B8A8Unorm;
    imageCreateInfo.tiling = ImageTiling::eLinear;
    imageCreateInfo.initialLayout = ImageLayout::eUndefined;
    imageCreateInfo.usage = ImageUsageFlagBits::eColorAttachment;
    imageCreateInfo.sharingMode = SharingMode::eExclusive;
    imageCreateInfo.samples = SampleCountFlagBits::e1;

    auto image = device->createImageUnique(imageCreateInfo);


    CommandBufferBeginInfo commandBufferBeginInfo;
    commandBuffers.at(0)->begin(commandBufferBeginInfo);
//
    commandBuffers.at(0)->end();

    CommandBuffer submitCommandBuffer[1]{commandBuffers.at(0).get()};
    SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = submitCommandBuffer;

    queue.submit({submitInfo}, nullptr);
    return 0;
}

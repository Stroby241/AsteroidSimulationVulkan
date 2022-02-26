#include "Buffer.h"
#include "Log.h"

Buffer::Buffer(Device& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
	: device(device),
	instanceSize(instanceSize),
	instanceCount(instanceCount),
	usageFlags(usageFlags),
	memoryPropertyFlags(memoryPropertyFlags)
{
	alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
	bufferSize = alignmentSize * instanceCount;
	device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}



Buffer::~Buffer() {

	unmap();
	vkDestroyBuffer(device.Getdevice(), buffer, nullptr);
	vkFreeMemory(device.Getdevice(), memory, nullptr);
}

VkDeviceSize Buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
{
	if (minOffsetAlignment > 0) {
		return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
	}
	return instanceSize;
}

VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset)
{
	return vkMapMemory(device.Getdevice(), memory, offset, size, 0, &mapped);
}

void Buffer::unmap()
{
	if (mapped) {
		vkUnmapMemory(device.Getdevice(), memory);
		mapped = nullptr;
	}
}

void Buffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
{
	if (mapped) {

		if (size == VK_WHOLE_SIZE) {
			memcpy(mapped, data, bufferSize);
		}
		else {
			char* memOffset = (char*)mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}
	else {
		VK_LOG_WARN("Cannot copy to unmapped buffer");
	}
}

VkResult Buffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkFlushMappedMemoryRanges(device.Getdevice(), 1, &mappedRange);
}

VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
{
	return VkDescriptorBufferInfo{ buffer,offset,size, };
}

VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkInvalidateMappedMemoryRanges(device.Getdevice(), 1, &mappedRange);
}

void Buffer::writeToIndex(void* data, int index)
{
	writeToBuffer(data, instanceSize, index * alignmentSize);
}

VkResult Buffer::flushIndex(int index)
{
	return flush(alignmentSize, index * alignmentSize);
}

VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(int index)
{
	return descriptorInfo(alignmentSize, index * alignmentSize);
}

VkResult Buffer::invalidateIndex(int index)
{
	return invalidate(alignmentSize, index * alignmentSize);
}



#include "Descriptor.h"
#include <assert.h>
#include "Log.h"
DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
	assert(bindings.count(binding) == 0 && "Binding already in use");
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = binding;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.descriptorCount = count;
	layoutBinding.stageFlags = stageFlags;
	bindings[binding] = layoutBinding;
	return *this;
}

std::unique_ptr<DescriptorSetLayout> DescriptorSetLayout::Builder::build() const
{
	return std::make_unique<DescriptorSetLayout>(device, bindings);
}


DescriptorSetLayout::DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
	: device(device), bindings(bindings)
{
	std::vector< VkDescriptorSetLayoutBinding> setLayoutBindings{};
	for (auto& kv : bindings) {
		setLayoutBindings.push_back(kv.second);
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
	descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutInfo.bindingCount = setLayoutBindings.size();
	descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

	if (vkCreateDescriptorSetLayout(device.Getdevice(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		VK_LOG_ERROR("failed to create descriptor set layout!");
		exit(0);
	}
}

DescriptorPool::Builder& DescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
	poolSize.push_back({ descriptorType,count });
	return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
{
	poolFlags = flags;
	return *this;
}


DescriptorPool::Builder& DescriptorPool::Builder::setMaxSets(uint32_t count)
{
	maxSets = count;
	return *this;
}

std::unique_ptr<DescriptorPool> DescriptorPool::Builder::build() const
{
	return std::make_unique< DescriptorPool>(device, maxSets, poolFlags, poolSize);
}


DescriptorSetLayout::~DescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(device.Getdevice(), descriptorSetLayout, nullptr);
}

DescriptorPool::DescriptorPool(Device& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector< VkDescriptorPoolSize>& poolSize) 
	: device(device)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = poolSize.size();
	descriptorPoolInfo.pPoolSizes = poolSize.data();
	descriptorPoolInfo.maxSets = maxSets;
	descriptorPoolInfo.flags = poolFlags;

	if (vkCreateDescriptorPool(device.Getdevice(), &descriptorPoolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		VK_LOG_ERROR("failed to create descriptor pool!");
		exit(0);
	}
}

DescriptorPool::~DescriptorPool()
{
	vkDestroyDescriptorPool(device.Getdevice(), descriptorPool, nullptr);
}

bool DescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
{
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;

	if (vkAllocateDescriptorSets(device.Getdevice(), &allocInfo, &descriptor) != VK_SUCCESS) {
		return false;
	}
	return true;
}

void DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
{
	vkFreeDescriptorSets(device.Getdevice(), descriptorPool, descriptors.size(), descriptors.data());
}

void DescriptorPool::resetPool()
{
	vkResetDescriptorPool(device.Getdevice(), descriptorPool, 0);
}

DesciptorWriter::DesciptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool)
	: setLayout(setLayout), pool(pool)
{
}
DesciptorWriter& DesciptorWriter::writeBuffer(
	uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
	assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

	auto& bindingDescription = setLayout.bindings[binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = binding;
	write.pBufferInfo = bufferInfo;
	write.descriptorCount = 1;

	writes.push_back(write);
	return *this;
}

DesciptorWriter& DesciptorWriter::writeImage(
	uint32_t binding, VkDescriptorImageInfo* imageInfo) {
	assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

	auto& bindingDescription = setLayout.bindings[binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = binding;
	write.pImageInfo = imageInfo;
	write.descriptorCount = 1;

	writes.push_back(write);
	return *this;
}

bool DesciptorWriter::build(VkDescriptorSet& set)
{
	bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
	if (!success) {
		return false;
	}
	overwrite(set);
	return true;
}

void DesciptorWriter::overwrite(VkDescriptorSet& set)
{
	for (auto& write : writes) {
		write.dstSet = set;
	}
	vkUpdateDescriptorSets(pool.device.Getdevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
}

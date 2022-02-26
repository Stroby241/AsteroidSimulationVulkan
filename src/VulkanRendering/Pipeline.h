#pragma once
#include <vector>
#include <fstream>
#include <filesystem>

#include "Device.h"


struct PipelineConfigInfo {
	VkViewport viewport;
	VkRect2D scissor;
	
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};


class Pipeline {


public:
	Pipeline(Device& device,
		const std::filesystem::path vertFile,
		const std::filesystem::path fragFile,
		const PipelineConfigInfo& configInfo);
	~Pipeline();

	Pipeline(const Pipeline&) = delete;
	void operator=(const Pipeline&) = delete;

	void bind(VkCommandBuffer commandBuffer);

	static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	VkPipeline graphicsPipeline;

private:
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	Device& device;
	

	std::vector<char> readFile(const std::filesystem::path& path);
	VkShaderModule CreateShader(const std::vector<char>& code);
	void CreateGrapficsPipeline(const std::filesystem::path& vertFile, const std::filesystem::path& fragFile, const PipelineConfigInfo& configInfo);
	
};    
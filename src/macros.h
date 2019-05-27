/*
* Global macros
*
* Copyright (C) 2018 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#ifdef VULKAN
#include "vulkan/vulkan.h"


#if defined(__ANDROID__)
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		LOGE("Fatal : VkResult is \" %d \" in %s at line %d", res, __FILE__, __LINE__);					\
		assert(res == VK_SUCCESS);																		\
	}																									\
}
#else
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << res << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
		assert(res == VK_SUCCESS);																		\
	}																									\
}
#endif

#endif//define vulkan

#define TIME(f,t)					\
{								\
								\
	auto function_time = std::chrono::high_resolution_clock::now();\
	f;			/*funcion */				\
	calculate_time(t,function_time);\
}			

#define ETIME(e,f,t)					\
{								\
								\
	auto function_time = std::chrono::high_resolution_clock::now();\
	f;			/*funcion */				\
	e->calculate_time(t,function_time);\
}		



#pragma once
#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "portable_executable.hpp"
#include "utils.hpp"
#include "nt.hpp"
#include "intel_driver.hpp"

#define PAGE_SIZE 0x1000

namespace kmdf_loader2
{
	//Note: if you set PassAllocationAddressAsFirstParam as true, param1 will be ignored
	uint64_t MapDriver(HANDLE iqvw64e_device_handle, BYTE* data);
	void RelocateImageByDelta(portable_executable::vec_relocs relocs, const uint64_t delta);
	bool FixSecurityCookie(void* local_image, uint64_t kernel_image_base);
	bool ResolveImports(HANDLE iqvw64e_device_handle, portable_executable::vec_imports imports);
	uint64_t AllocIndependentPages(HANDLE device_handle, uint32_t size);
	uint64_t AllocMdlMemory(HANDLE iqvw64e_device_handle, uint64_t size, uint64_t* mdlPtr);
}
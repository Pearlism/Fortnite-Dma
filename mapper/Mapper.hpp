#include "kmdf_loader2.hpp"
#include "../SkCrypt.h"

bool vMapDriver(std::vector<uint8_t> VectorData) {
	HANDLE iqvw64e_device_handle = intel_driver::Load();

	if (iqvw64e_device_handle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if (!kmdf_loader2::MapDriver(iqvw64e_device_handle, VectorData.data())) {
		Log(L"[-] Failed to map " << driver_path << std::endl);
		intel_driver::Unload(iqvw64e_device_handle);
		return 0;
	}

	if (!intel_driver::Unload(iqvw64e_device_handle)) {
		Log(L"[-] Warning failed to fully unload vulnerable driver " << std::endl);
	}
	Log(L"[+] success" << std::endl);
	return 1;
}
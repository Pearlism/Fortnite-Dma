
#include "Registry.h"
#include "Memory.h"

std::string c_registry::QueryValue(const char* path, e_registry_type type)
{
	if (!driver->vHandle)
		return "";

	BYTE buffer[0x128];
	DWORD _type = static_cast<DWORD>(type);
	DWORD size = sizeof(buffer);

	if (!VMMDLL_WinReg_QueryValueExU(driver->vHandle, const_cast<LPSTR>(path), &_type, buffer, &size))
	{
		LOG("[!] failed QueryValueExU call\n");
		return "";
	}

	std::wstring wstr = std::wstring(reinterpret_cast<wchar_t*>(buffer));
	return std::string(wstr.begin(), wstr.end());
}

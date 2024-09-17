

#include "InputManager.h"
#include "Registry.h"
#include "Memory.h"
#include <windows.h>

#include <winnt.h>

//enum class e_registry_type
//{
//	none = REG_NONE,
//	sz = REG_SZ,
//	expand_sz = REG_EXPAND_SZ,
//	binary = REG_BINARY,
//	dword = REG_DWORD,
//	dword_little_endian = REG_DWORD_LITTLE_ENDIAN,
//	dword_big_endian = REG_DWORD_BIG_ENDIAN,
//	link = REG_LINK,
//	multi_sz = REG_MULTI_SZ,
//	resource_list = REG_RESOURCE_LIST,
//	full_resource_descriptor = REG_FULL_RESOURCE_DESCRIPTOR,
//	resource_requirements_list = REG_RESOURCE_REQUIREMENTS_LIST,
//	qword = REG_QWORD,
//	qword_little_endian = REG_QWORD_LITTLE_ENDIAN
//};
//
//class c_registry
//{
//private:
//public:
//	c_registry()
//	{
//	}
//
//	~c_registry()
//	{
//	}
//
//	std::string QueryValue(const char* path, e_registry_type type);
//};
//
//
//class c_keys
//{
//private:
//	uint64_t gafAsyncKeyStateExport = 0;
//	uint8_t state_bitmap[64]{ };
//	uint8_t previous_state_bitmap[256 / 8]{ };
//	uint64_t win32kbase = 0;
//
//	int win_logon_pid = 0;
//
//	c_registry registry;
//	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
//
//public:
//	c_keys() = default;
//
//	~c_keys() = default;
//
//	bool InitKeyboard();
//
//	void UpdateKeys();
//	bool IsKeyDown(uint32_t virtual_key_code);
//};


//TODO: Restart winlogon.exe when it doesn't exist.
bool c_keys::InitKeyboard()
{
	std::string win = registry.QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;

	this->win_logon_pid = driver->GetPidFromName("winlogon.exe");
	if (Winver > 22000)
	{
		auto pids = driver->GetPidListFromName("csrss.exe");
		for (size_t i = 0; i < pids.size(); i++)
		{
			auto pid = pids[i];
			uintptr_t tmp = VMMDLL_ProcessGetModuleBaseU(driver->vHandle, pid, const_cast<LPSTR>("win32ksgd.sys"));
			uintptr_t g_session_global_slots = tmp + 0x3110;
			uintptr_t user_session_state = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(g_session_global_slots, pid), pid), pid);
			gafAsyncKeyStateExport = user_session_state + 0x3690;
			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
				break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
	else
	{
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		bool result = VMMDLL_Map_GetEATU(driver->vHandle, driver->GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, const_cast<LPSTR>("win32kbase.sys"), &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}

		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;

				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		{
			PVMMDLL_MAP_MODULEENTRY module_info;
			auto result = VMMDLL_Map_GetModuleFromNameW(driver->vHandle, driver->GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, static_cast<LPCWSTR>(L"win32kbase.sys"), &module_info, VMMDLL_MODULE_FLAG_NORMAL);
			if (!result)
			{
				LOG("failed to get module info\n");
				return false;
			}

			char str[32];
			if (!VMMDLL_PdbLoad(driver->vHandle, driver->GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, module_info->vaBase, str))
			{
				LOG("failed to load pdb\n");
				return false;
			}

			uintptr_t gafAsyncKeyState;
			if (!VMMDLL_PdbSymbolAddress(driver->vHandle, str, const_cast<LPSTR>("gafAsyncKeyState"), &gafAsyncKeyState))
			{
				LOG("failed to find gafAsyncKeyState\n");
				return false;
			}
			LOG("found gafAsyncKeyState at: 0x%p\n", gafAsyncKeyState);
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
}

void c_keys::UpdateKeys()
{
	uint8_t previous_key_state_bitmap[64] = {0};
	memcpy(previous_key_state_bitmap, state_bitmap, 64);

	VMMDLL_MemReadEx(driver->vHandle, this->win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, reinterpret_cast<PBYTE>(&state_bitmap), 64, NULL, VMMDLL_FLAG_NOCACHE);
	for (int vk = 0; vk < 256; ++vk)
		if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
			previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}

bool c_keys::IsKeyDown(uint32_t virtual_key_code)
{
	if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		return false;
	if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(1))
	{
		UpdateKeys();
		start = std::chrono::system_clock::now();
	}
	return state_bitmap[(virtual_key_code * 2 / 8)] & 1 << virtual_key_code % 4 * 2;
}

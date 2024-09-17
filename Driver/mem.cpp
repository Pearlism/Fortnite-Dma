#include "mem.h"
#include <iostream>
#include <tlhelp32.h>
#include <winioctl.h>
#include <winternl.h>
#include "../GlobalDefine.h"
#pragma comment(lib, "ntdll.lib")
//#define comm_key 0xaf21fe4e3c1a57a


#ifdef  Driver
bool mem::startup()
{
	if (!GlobalFindAtomA("/"))
	{
		HKEY hKey;
		std::string keyPath = "SOFTWARE\\CPUID";
		std::string valueName = "UUID";

		GUID newUUID;
		CoCreateGuid(&newUUID);

		snprintf(this->uuidString, sizeof(this->uuidString), "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", newUUID.Data1, newUUID.Data2, newUUID.Data3, newUUID.Data4[0], newUUID.Data4[1], newUUID.Data4[2], newUUID.Data4[3], newUUID.Data4[4], newUUID.Data4[5], newUUID.Data4[6], newUUID.Data4[7]);

		if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
		{
			if (RegSetValueExA(hKey, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(this->uuidString),
				static_cast<DWORD>(strlen(this->uuidString) + 1)) == ERROR_SUCCESS)
			{
				GlobalAddAtomA("/");
			}
			RegCloseKey(hKey);
		}
	}
	else
	{
		HKEY hKey;
		std::string keyPath = "SOFTWARE\\CPUID";
		std::string valueName = "UUID";

		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			DWORD dataSize = 0;
			if (RegQueryValueExA(hKey, valueName.c_str(), 0, NULL, NULL, &dataSize) == ERROR_SUCCESS)
			{
				char* buffer = new char[dataSize];
				if (RegQueryValueExA(hKey, valueName.c_str(), 0, NULL, reinterpret_cast<LPBYTE>(buffer), &dataSize) == ERROR_SUCCESS)
				{
					strncpy(this->uuidString, buffer, sizeof(this->uuidString) - 1);
					this->uuidString[sizeof(this->uuidString) - 1] = '\0';
				}
				delete[] buffer;
			}

			RegCloseKey(hKey);
		}
	}

}


int mem::launch()
{
	std::string name = std::string("\\\\.\\\/") + this->uuidString;
	this->drv_handle = CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (!this->drv_handle || this->drv_handle == INVALID_HANDLE_VALUE)
	{
		return 6;
	}

	return 0;
}

bool mem::attach()
{
	this->data.my_pid = GetCurrentProcessId();

	//std::cout << "[+] this->pid: " << this->pid << std::endl;

	Sleep(10);

	if (!this->update_base())
	{
		MessageBoxA(0, ("failed to get module base"), ("gay"), MB_OK);

		//std::cout << "[-] failed to get game module base\n";

		return false;
	}

	//std::cout << "[+] this->base: 0x" << std::hex << this->base << std::endl;

	Sleep(10);

	if (!this->update_cr3())
	{
		MessageBoxA(0, ("failed to get cr3"), ("gay"), MB_OK);

		//std::cout << "[-] failed to get game cr3\n";

		return false;
	}

	//std::cout << "[+] this->cr3: 0x" << std::hex << this->cr3 << std::endl;

	//system("pause");

	return true;
}

bool mem::ping()
{

}

void mem::req(comm_data* data)
{
	//DeviceIoControl(this->drv_handle, control_code, data, sizeof(comm_data), nullptr, 0, NULL, NULL);
}


uintptr_t mem::allocate_memory(SIZE_T size, DWORD Protect)
{
	this->data.pid = this->pid;
	this->data.size = size;
	this->data.buffer = Protect;
	this->data.type = 8;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), &this->data, sizeof(comm_data), 0, 0);

	return this->data.address;
}


uintptr_t mem::protect_memory(uintptr_t address, SIZE_T size, DWORD Protect)
{
	this->data.pid = this->pid;
	this->data.address = address;
	this->data.size = size;
	this->data.buffer = Protect;
	this->data.type = 11;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), &this->data, sizeof(comm_data), 0, 0);

	return this->data.buffer;
}


void mem::write_mem(uintptr_t address, uintptr_t buffer, int size)
{
	//comm_data data = {};

	//data.cr3 = this->cr3;
	this->data.address = address;
	this->data.buffer = buffer;
	this->data.size = size;
	this->data.type = 2;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), 0, 0, 0, 0);
}

void mem::write_virtual_mem(uintptr_t address, uintptr_t buffer, int size)
{
	//comm_data data = {};

	//this->data.cr3 = this->cr3;
	this->data.address = address;
	this->data.buffer = buffer;
	this->data.size = size;
	this->data.type = 10;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), 0, 0, 0, 0);
}

void mem::read_mem(uintptr_t address, uintptr_t buffer, int size)
{
	//comm_data data = {};

	//data.cr3 = this->cr3;
	this->data.address = address;
	this->data.buffer = buffer;
	this->data.size = size;
	this->data.type = 1;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), 0, 0, 0, 0);
}

bool mem::update_cr3()
{
	//comm_data data = {};

	this->data.address = this->base;
	this->data.type = 3;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), &this->data, sizeof(comm_data), 0, 0);

	this->cr3 = data.cr3;

	return this->cr3 > 0;
}

bool mem::update_base()
{
	//comm_data data = {};

	this->data.pid = this->pid;
	this->data.type = 4;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), &this->data, sizeof(comm_data), 0, 0);

	this->base = data.address;

	return this->base > 0;
}

bool mem::set_pid(int owner_pid, int pid_to_set)
{
	//comm_data data = {};

	this->data.pid = owner_pid;
	this->data.address = pid_to_set;
	this->data.type = 9;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), &this->data, sizeof(comm_data), 0, 0);

	return true;
}

bool mem::hide_window(HWND hide)
{
	//comm_data data = {};

	this->data.address = uintptr_t(hide);
	this->data.type = 8;

	DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), &this->data, sizeof(comm_data), 0, 0);

	return true;
}

int mem::query_pid_by_process_name(const wchar_t* process_name)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(0x00000002, 0);
	if (!snapshot || snapshot == INVALID_HANDLE_VALUE)
	{
		//std::cout << "[-] failed to take process snapshot\n";

		return 0;
	}

	PROCESSENTRY32W entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32W);

	while (Process32NextW(snapshot, &entry))
	{
		if (wcsstr(entry.szExeFile, process_name))
		{
			CloseHandle(snapshot);

			return entry.th32ProcessID;
		}
	}

	CloseHandle(snapshot);

	return 0;
}

#endif //  CurrentReadMethod
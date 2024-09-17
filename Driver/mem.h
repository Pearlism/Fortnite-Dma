#pragma once

#include <windows.h>
#include <corecrt_wstring.h>
#include <string>
#include <vector>
#include "../GlobalDefine.h"
//#define DMA

#define control_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x12, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

//struct comm_data
//{
//	/* comm key */
//	//volatile uintptr_t key;
//
//	/* process id of cheat process */
//	volatile int my_pid;
//
//	/* game's process id*/
//	volatile int pid;
//
//	/* game's control register 3 */
//	volatile uintptr_t cr3;
//
//	/* target address (mostly used for r/w) */
//	volatile uintptr_t address;
//
//	/* copy buffer from cheat (mostly used for r/w) */
//	volatile uintptr_t buffer;
//
//	/* count in bytes (mostly used for r/w) */
//	volatile uintptr_t size;
//
//	/* type of the request */
//	volatile int request_type;
//
//	/* mouse */
//	volatile int x;
//	volatile int y;
//	volatile unsigned short button_flags;
//
//	/* wether driver currently is operating a request */
//	volatile bool running_request;
//};

#include <limits>
#include <climits>
#include <random>

template <typename T>
T getRandomNumber(T min, T max)
{
	std::random_device rnd;
	std::mt19937_64 gen(rnd());

	if constexpr (std::is_floating_point_v<T>)
	{
		std::uniform_real_distribution<T> dis(min, max);
		return dis(gen);
	}
	else if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> dis(min, max);
		return dis(gen);
	}
	else
	{
	}
}


inline bool TestAddress2(uintptr_t address)
{
	if ((void*)address == nullptr) { return false; }
	if (address > 0x7FFFFFFFFFFF) { return false; }
	if (!address) { return false; }
	if (address <= 0x10000) { return false; }
	if (address > 0xF000000000000) { return false; }
	if (address == 0x00000000000000b0) { return false; }
	if (address >= 0xffffffffffffffff) { return false; }
	if (address <= 0x000000000000FFFF) { return false; }
	if (address == 0x00000000000002e8) { return false; }
	if ((uintptr_t*)address == nullptr) { return false; }
	return true;
}

#ifdef Driver



struct comm_data
{
	int type = 0;
	int pid = 0;
	int my_pid = 0;

	uintptr_t cr3 = 0ull;
	uintptr_t address = 0ull;
	uintptr_t buffer = 0ull;
	uintptr_t size = 0ull;

	int x = 0;
	int y = 0;
	short flags = 0;
};

//comm_data[2]
//1 = normal
//2 = cache
//alloc new mem* in cache.h

class mem
{

public:

	HANDLE drv_handle;

	char uuidString[39];

	/* game's process id */
	int pid = 0;

	/* game's module base */
	uintptr_t base = 0ull;

	/* game's control register 3 (for translating virtual <-> physical) */
	uintptr_t cr3 = 0ull;

	/* games's peb (for iterating game modules) - NOT DONE YET */
	uintptr_t peb = 0ull;

	/* pointer to our shared memory */
	//volatile comm_data* shared_data = nullptr;

	comm_data data = {};

public:

	void call_mouse(int x, int y, unsigned short buttonflags)
	{
		this->data.x = x;
		this->data.y = y;
		this->data.flags = buttonflags;
		this->data.type = 6;

		DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), 0, 0, 0, 0);
	}

	void call_keyboard(int code, int flags)
	{
		this->data.x = code;
		this->data.y = flags;
		this->data.type = 7;

		DeviceIoControl(this->drv_handle, control_code, &this->data, sizeof(comm_data), 0, 0, 0, 0);
	}

	bool startup();

	/* opens handle to the shared memory and assings the pointer to this->shared_data */
	int launch();

	/* attaches the driver to the target process (fortnite) */
	bool attach();

	/* if the driver was ran successful, this function should return true */
	bool ping();

	/* do a request to the driver with the before given info */
	void req(comm_data* data);

	/* returns the process id of a process */
	int query_pid_by_process_name(const wchar_t* process_name);

	/* updates this->cr3 and shared_data->cr3 */
	bool update_cr3();

	/* updates this->base and shared_data->base */
	bool update_base();

	/* updates owner pid */
	bool set_pid(int owner_pid, int pid_to_set);

	/* hide window */
	bool hide_window(HWND hide);

	/* r/w from/to the game process  */
	void read_mem(uintptr_t address, uintptr_t buffer, int size);

	/* wrapper for this->read_mem */
	void read_mem(uintptr_t address, void* buffer, int size)
	{
		if (!TestAddress2(address)) { return; }
		this->read_mem(address, (uintptr_t)buffer, size);
	}

	void read_mem(HANDLE VAL, uintptr_t address, void* buffer, int size)
	{
		if (!TestAddress2(address)) { return; }
		this->read_mem(address, (uintptr_t)buffer, size);
	}

	void read_mem(HANDLE VAL, uintptr_t address, uintptr_t buffer, int size)
	{
		if (!TestAddress2(address)) { return; }
		this->read_mem(address, (uintptr_t)buffer, size);
	}

	/* simple reading template. will be inlined */
	template<typename type = uintptr_t>
	type read(uintptr_t address)
	{
		type buffer = {};
		if (!TestAddress2(address)) { return buffer; }
		this->read_mem(address, &buffer, sizeof(type));

		return buffer;
	}

	/* wrapper for this->read */
	template<typename type = uintptr_t>
	type read(void* address)
	{
		return this->read<type>((uintptr_t)address);
	}
	uintptr_t allocate_memory(SIZE_T size, DWORD Protect);
	uintptr_t protect_memory(uintptr_t address, SIZE_T size, DWORD Protect);

	void write_mem(uintptr_t address, uintptr_t buffer, int size);

	void write_mem(uintptr_t address, void* buffer, int size)
	{
		if (!TestAddress2(address)) { return; }
		this->write_mem(address, (uintptr_t)buffer, size);
	}

	void write_virtual_mem(uintptr_t address, uintptr_t buffer, int size);

	void write_virtual_mem(uintptr_t address, void* buffer, int size)
	{
		this->write_virtual_mem(address, (uintptr_t)buffer, size);
	}

	/* simple writing template. will be inlined */
	template <typename type>
	void write_virtual(uintptr_t address, type value)
	{
		type buffer = {};

		this->read_mem(address, &buffer, sizeof(type));

		/* only write if value is different */
		if (memcmp((void*)&value, (void*)&buffer, sizeof(type)) != 0)
		{
			this->write_virtual_mem(address, &value, sizeof(type));
		}
	}


	/* simple writing template. will be inlined */
	template <typename type>
	void write(uintptr_t address, type value)
	{

		type buffer = {};
		if (!TestAddress2(address)) { return; }
		this->read_mem(address, &buffer, sizeof(type));

		/* only write if value is different */
		if (memcmp((void*)&value, (void*)&buffer, sizeof(type)) != 0)
		{
			this->write_mem(address, &value, sizeof(type));
		}
	}

	/* wrapper for this->write */
	template<typename type>
	void write(void* address, type value)
	{
		if (!TestAddress2((uintptr_t)address)) { return; }
		this->write<type>((uintptr_t)address, value);
	}

	/* returns the target process' base */
	uintptr_t get_base()
	{
		return this->base;
	}

	/* returns the target process' control register 3 */
	uintptr_t get_cr3()
	{
		return this->cr3;
	}

	/* returns the target process' process id */
	uintptr_t get_pid()
	{
		return this->pid;
	}

	struct ScatterData {
		uintptr_t Address;
		void* Buffer;
		int64_t Size;
	};

	struct ScatterReguest {
		HANDLE Handle = 0;
		std::vector<ScatterData> ScattersData = {};
	};

	std::vector<ScatterReguest*> ScattersReguest = {};

	HANDLE CreateScatterHandle()
	{
		ScatterReguest* NewScatter = new ScatterReguest;
		NewScatter->Handle = (HANDLE)getRandomNumber(1, 10000000);
		ScattersReguest.push_back(NewScatter);
		return NewScatter->Handle;
	}

	ScatterReguest* GetScatterRequestByHandle(HANDLE Handle)
	{
		for (int64_t i = 0; i < ScattersReguest.size(); i++)
		{
			if (ScattersReguest[i]->Handle == Handle)
			{
				return ScattersReguest[i];
			}
		}
		return nullptr;
	}

	bool AddScatterReadRequest(HANDLE Handle, uintptr_t Address, void* Buffer, int64_t Size)
	{
		ScatterReguest* CurrentScatter = GetScatterRequestByHandle(Handle);
		if (CurrentScatter)
		{
			ScatterData NewScatterData;
			NewScatterData.Address = Address;
			NewScatterData.Buffer = Buffer;
			NewScatterData.Size = Size;
			CurrentScatter->ScattersData.push_back(NewScatterData);
			return 1;
		}
		else
		{
			return 0;
		}
	}

	bool ExecuteReadScatter(HANDLE Handle)
	{
		ScatterReguest* CurrentScatter = GetScatterRequestByHandle(Handle);
		if (CurrentScatter)
		{
			for (int64_t i = 0; i < CurrentScatter->ScattersData.size(); i++)
			{
				read_mem(CurrentScatter->ScattersData[i].Address, CurrentScatter->ScattersData[i].Buffer, CurrentScatter->ScattersData[i].Size);
			}
			return 1;

		}
		else
		{
			return 0;
		}
	}

	bool CloseScatterHandle(HANDLE Handle)
	{
		ScatterReguest* CurrentScatter = GetScatterRequestByHandle(Handle);
		if (CurrentScatter)
		{
			auto it = std::find(ScattersReguest.begin(), ScattersReguest.end(), CurrentScatter);
			if (it != ScattersReguest.end())
			{
				it = ScattersReguest.erase(it);
			}

			delete CurrentScatter;
			return 1;
		}
		return 0;

	}
};

static mem* driver = new mem;
#endif // Driver



#pragma once


#ifndef PCH_H
#define PCH_H

//DMA
#include "vmmdll.h"

#include <Windows.h>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <filesystem>

#define DEBUG_INFO
#ifdef DEBUG_INFO
#define LOG(fmt, ...) std::printf(fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) std::wprintf(fmt, ##__VA_ARGS__)
#else
#define LOG
#define LOGW
#endif

#define THROW_EXCEPTION
#ifdef THROW_EXCEPTION
#define THROW(fmt, ...) throw std::runtime_error(fmt, ##__VA_ARGS__)
#endif

#endif //PCH_H

//#include "InputManager.h"
//#include "Registry.h"
#include "Shellcode.h"
#include "structs.h"
#include <winternl.h>

//#include <winnt.h>


//typedef struct _PEB_LDR_DATA
//{
//	BYTE Reserved1[8];
//	PVOID Reserved2[3];
//	LIST_ENTRY InMemoryOrderModuleList;
//} PEB_LDR_DATA, * PPEB_LDR_DATA;
//
//typedef struct _UNICODE_STRING
//{
//	USHORT Length;
//	USHORT MaximumLength;
//	PWSTR Buffer;
//} UNICODE_STRING, * PUNICODE_STRING;
//
//typedef struct _LDR_DATA_TABLE_ENTRY
//{
//	PVOID Reserved1[2];
//	LIST_ENTRY InMemoryOrderLinks;
//	PVOID Reserved2[2];
//	PVOID DllBase;
//	PVOID Reserved3[2];
//	UNICODE_STRING FullDllName;
//	BYTE Reserved4[8];
//	PVOID Reserved5[3];
//#pragma warning(push)
//#pragma warning(disable: 4201) // we'll always use the Microsoft compiler
//	union
//	{
//		ULONG CheckSum;
//		PVOID Reserved6;
//	} DUMMYUNIONNAME;
//#pragma warning(pop)
//	ULONG TimeDateStamp;
//} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;
//
//typedef struct _RTL_USER_PROCESS_PARAMETERS
//{
//	BYTE Reserved1[16];
//	PVOID Reserved2[10];
//	UNICODE_STRING ImagePathName;
//	UNICODE_STRING CommandLine;
//} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;
//
////This function pointer is undocumented and just valid for windows 2000. Therefore I guess. 
//typedef VOID(WINAPI* PPS_POST_PROCESS_INIT_ROUTINE)(VOID);
//
//typedef struct _PEB
//{
//	BYTE reserved_0[2];
//	BYTE is_debugging;
//	BYTE reserved_1[13];
//	uint64_t image;
//	PPEB_LDR_DATA Ldr;
//	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
//	PVOID Reserved4[3];
//	PVOID AtlThunkSListPtr;
//	PVOID Reserved5;
//	ULONG Reserved6;
//	PVOID Reserved7;
//	ULONG Reserved8;
//	ULONG AtlThunkSListPtr32;
//	PVOID Reserved9[45];
//	BYTE Reserved10[96];
//	PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
//	BYTE Reserved11[128];
//	PVOID Reserved12[1];
//	ULONG SessionId;
//} PEB, * PPEB;


enum class e_registry_type
{
	none = REG_NONE,
	sz = REG_SZ,
	expand_sz = REG_EXPAND_SZ,
	binary = REG_BINARY,
	dword = REG_DWORD,
	dword_little_endian = REG_DWORD_LITTLE_ENDIAN,
	dword_big_endian = REG_DWORD_BIG_ENDIAN,
	link = REG_LINK,
	multi_sz = REG_MULTI_SZ,
	resource_list = REG_RESOURCE_LIST,
	full_resource_descriptor = REG_FULL_RESOURCE_DESCRIPTOR,
	resource_requirements_list = REG_RESOURCE_REQUIREMENTS_LIST,
	qword = REG_QWORD,
	qword_little_endian = REG_QWORD_LITTLE_ENDIAN
};

class c_registry
{
private:
public:
	c_registry()
	{
	}

	~c_registry()
	{
	}

	std::string QueryValue(const char* path, e_registry_type type);
};


class c_keys
{
private:
	uint64_t gafAsyncKeyStateExport = 0;
	uint8_t state_bitmap[64]{ };
	uint8_t previous_state_bitmap[256 / 8]{ };
	uint64_t win32kbase = 0;

	int win_logon_pid = 0;

	c_registry registry;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

public:
	c_keys() = default;

	~c_keys() = default;

	bool InitKeyboard();

	void UpdateKeys();
	bool IsKeyDown(uint32_t virtual_key_code);
};


class Memory
{

private:
	struct LibModules
	{
		HMODULE VMM = nullptr;
		HMODULE FTD3XX = nullptr;
		HMODULE LEECHCORE = nullptr;
	};

	static inline LibModules modules { };

	struct CurrentProcessInformation
	{
		int PID = 0;
		size_t base_address = 0;
		size_t base_size = 0;
		std::string process_name = "";
	};

	static inline CurrentProcessInformation current_process { };

	static inline BOOLEAN DMA_INITIALIZED = FALSE;
	static inline BOOLEAN PROCESS_INITIALIZED = FALSE;
	/**
	*Dumps the systems Current physical memory pages
	*To a file so we can use it in our DMA (:
	*This file it created to %temp% folder
	*@return true if successful, false if not.
	*/


	bool DumpMemoryMap(bool debug = false);

	/**
	* brief Removes basic information related to the FPGA device
	* This is required before any DMA operations can be done.
	* To ensure the optimal safety in game cheating.
	* @return true if successful, false if not.
	*/
	bool SetFPGA();

	//shared pointer
	std::shared_ptr<c_keys> key;
	c_registry registry;
	//c_shellcode shellcode;

	/*this->registry_ptr = std::make_shared<c_registry>(*this);
	this->key_ptr = std::make_shared<c_keys>(*this);*/

public:

	uintptr_t base = 0;

	bool InitMemory();

	/**
	 * brief Constructor takes a wide string of the process.
	 * Expects that all the libraries are in the root dir
	 */

	// Memory();
	//~Memory();

	/**
	* @brief Gets the registry object
	* @return registry class
	*/
	c_registry GetRegistry() { return registry; }

	/**
	* @brief Gets the key object
	* @return key class
	*/
	c_keys* GetKeyboard() { return key.get(); }

	/**
	* @brief Gets the shellcode object
	* @return shellcode class
	*/
	//c_shellcode GetShellcode() { return shellcode; }

	/**
	* brief Initializes the DMA
	* This is required before any DMA operations can be done.
	* @param process_name the name of the process
	* @param memMap if true, will dump the memory map to a file	& make the DMA use it.
	* @return true if successful, false if not.
	*/
	bool Init(std::string process_name, bool memMap = true, bool debug = false);

	/*This part here is things related to the process information such as Base daddy, Size ect.*/

	/**
	* brief Gets the process id of the process
	* @param process_name the name of the process
	* @return the process id of the process
	*/
	DWORD GetPidFromName(std::string process_name);

	/**
	* brief Gets all the processes id(s) of the process
	* @param process_name the name of the process
	* @returns all the processes id(s) of the process
	*/
	std::vector<int> GetPidListFromName(std::string process_name);

	/**
	* \brief Gets the module list of the process
	* \param process_name the name of the process 
	* \return all the module names of the process 
	*/
	std::vector<std::string> GetModuleList(std::string process_name);

	/**
	* \brief Gets the process information
	* \return the process information
	*/
	VMMDLL_PROCESS_INFORMATION GetProcessInformation();

	/**
	* \brief Gets the process peb
	* \return the process peb 
	*/
	PEB GetProcessPeb();

	/**
	* brief Gets the base address of the process
	* @param module_name the name of the module
	* @return the base address of the process
	*/
	size_t GetBaseDaddy(std::string module_name);

	/**
	* brief Gets the base size of the process
	* @param module_name the name of the module
	* @return the base size of the process
	*/
	size_t GetBaseSize(std::string module_name);

	/**
	* brief Gets the export table address of the process
	* @param import the name of the export
	* @param process the name of the process
	* @param module the name of the module that you wanna find the export in
	* @return the export table address of the export
	*/
	uintptr_t GetExportTableAddress(std::string import, std::string process, std::string module);

	/**
	* brief Gets the import table address of the process
	* @param import the name of the import
	* @param process the name of the process
	* @param module the name of the module that you wanna find the import in
	* @return the import table address of the import
	*/
	uintptr_t GetImportTableAddress(std::string import, std::string process, std::string module);

	/**
	 * \brief This fixes the CR3 fuckery that EAC does.
	 * It fixes it by iterating over all DTB's that exist within your system and looks for specific ones
	 * that nolonger have a PID assigned to them, aka their pid is 0
	 * it then puts it in a vector to later try each possible DTB to find the DTB of the process.
	 * NOTE: Using FixCR3 requires you to have symsrv.dll, dbghelp.dll and info.db
	 */
	bool FixCr3();

	/**
	 * \brief Dumps the process memory at address (requires to be a valid PE Header) to the path
	 * \param address the address to the PE Header(BaseAddress)
	 * \param path the path where you wanna save dump to
	 */
	bool DumpMemory(uintptr_t address, std::string path);

	/*This part is where all memory operations are done, such as read, write.*/

	/**
	 * \brief Scans the process for the signature.
	 * \param signature the signature example "48 ? ? ?"
	 * \param range_start Region to start scan from 
	 * \param range_end Region up to where it should scan
	 * \param PID (OPTIONAL) where to read to?
	 * \return address of signature
	 */
	uint64_t FindSignature(const char* signature, uint64_t range_start, uint64_t range_end, int PID = 0);

	/**
	 * \brief Writes memory to the process 
	 * \param address The address to write to
	 * \param buffer The buffer to write
	 * \param size The size of the buffer
	 * \return 
	 */
	bool write(uintptr_t address, void* buffer, size_t size) const;
	bool write(uintptr_t address, void* buffer, size_t size, int pid) const;

	/**
	 * \brief Writes memory to the process using a template
	 * \param address to write to
	 * \param value the value you'll write to the address
	 */
	template <typename T>
	void write(void* address, T value)
	{
		write(address, &value, sizeof(T));
	}

	template <typename T>
	void write(uintptr_t address, T value)
	{
		write(address, &value, sizeof(T));
	}

	/**
	* brief Reads memory from the process
	* @param address The address to read from
	* @param buffer The buffer to read to
	* @param size The size of the buffer
	* @return true if successful, false if not.
	*/
	bool read(uintptr_t address, void* buffer, size_t size) const;
	bool read_mem(uintptr_t address, void* buffer, size_t size) const;
	bool read(uintptr_t address, void* buffer, size_t size, int pid) const;

	/**
	* brief Reads memory from the process using a template
	* @param address The address to read from
	* @return the value read from the process
	*/
	template <typename T>
	T read(void* address)
	{
		T buffer { };
		memset(&buffer, 0, sizeof(T));
		read(reinterpret_cast<uint64_t>(address), reinterpret_cast<void*>(&buffer), sizeof(T));

		return buffer;
	}

	template <typename T>
	T read(uint64_t address)
	{
		return read<T>(reinterpret_cast<void*>(address));
	}

	/**
	* brief Reads memory from the process using a template and pid
	* @param address The address to read from
	* @param pid The process id of the process
	* @return the value read from the process
	*/
	template <typename T>
	T read(void* address, int pid)
	{
		T buffer { };
		memset(&buffer, 0, sizeof(T));
		read(reinterpret_cast<uint64_t>(address), reinterpret_cast<void*>(&buffer), sizeof(T), pid);

		return buffer;
	}

	template <typename T>
	T read(uint64_t address, int pid)
	{
		return read<T>(reinterpret_cast<void*>(address), pid);
	}

	/**
	* brief Reads a chain of offsets from the address
	* @param address The address to read from
	* @param a vector of offset values to read through
	* @return the value read from the chain
	*/
	uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets)
	{
		uint64_t result = read<uint64_t>(base + offsets.at(0));
		for (int i = 1; i < offsets.size(); i++) result = read<uint64_t>(result + offsets.at(i));
		return result;
	}

	/**
	 * \brief Create a scatter handle, this is used for scatter read/write requests
	 * \return Scatter handle
	 */
	VMMDLL_SCATTER_HANDLE CreateScatterHandle() const;
	VMMDLL_SCATTER_HANDLE CreateScatterHandle(int pid) const;

	/**
	 * \brief Closes the scatter handle
	 * \param handle
	 */
	void CloseScatterHandle(VMMDLL_SCATTER_HANDLE handle);

	/**
	 * \brief Adds a scatter read/write request to the handle
	 * \param handle the handle
	 * \param address the address to read/write to 
	 * \param buffer the buffer to read/write to
	 * \param size the size of buffer
	 */
	void AddScatterReadRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size);
	void AddScatterWriteRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size);

	/**
	 * \brief Executes all prepared scatter requests, note if you created a scatter handle with a pid
	 * you'll need to specify the pid in the execute function. so we can clear the scatters from the handle.
	 * \param handle 
	 * \param pid 
	 */
	void ExecuteReadScatter(VMMDLL_SCATTER_HANDLE handle, int pid = 0);
	void ExecuteWriteScatter(VMMDLL_SCATTER_HANDLE handle, int pid = 0);

	/*the FPGA handle*/
	VMM_HANDLE vHandle;
};

//inline Memory mem;

inline Memory* driver = new Memory;
//inline Memory* driver = mem;


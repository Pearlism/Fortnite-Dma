#include "kmdf_loader2.hpp"

#define PAGE_SIZE 0x1000
#define PAGE_2MB_SIZE PAGE_SIZE * 512
#define PAGE_2MB_SHIFT 22ull
#define PAGE_ALIGN_2MB(_VAR_) (((DWORD64)_VAR_) & 0xFFFFFFFFFFE00000ULL)
#define ADDRMASK_EPT_PML2_OFFSET(_VAR_) (((DWORD64)_VAR_) & 0x1FFFFFULL)

uint64_t kmdf_loader2::AllocMdlMemory(HANDLE iqvw64e_device_handle, uint64_t size, uint64_t* mdlPtr) {
	/*added by psec*/
	LARGE_INTEGER LowAddress, HighAddress;
	LowAddress.QuadPart = 0;
	HighAddress.QuadPart = 0xffff'ffff'ffff'ffffULL;

	uint64_t pages = (size / PAGE_SIZE) + 1;
	auto mdl = intel_driver::MmAllocatePagesForMdl(iqvw64e_device_handle, LowAddress, HighAddress, LowAddress, pages * (uint64_t)PAGE_SIZE);
	if (!mdl) {
		Log(L"[-] Can't allocate pages for mdl" << std::endl);
		return { 0 };
	}

	uint32_t byteCount = 0;
	if (!intel_driver::ReadMemory(iqvw64e_device_handle, mdl + 0x028 /*_MDL : byteCount*/, &byteCount, sizeof(uint32_t))) {
		Log(L"[-] Can't read the _MDL : byteCount" << std::endl);
		return { 0 };
	}

	if (byteCount < size) {
		Log(L"[-] Couldn't allocate enough memory, cleaning up" << std::endl);
		intel_driver::MmFreePagesFromMdl(iqvw64e_device_handle, mdl);
		intel_driver::FreePool(iqvw64e_device_handle, mdl);
		return { 0 };
	}

	auto mappingStartAddress = intel_driver::MmMapLockedPagesSpecifyCache(iqvw64e_device_handle, mdl, nt::KernelMode, nt::MmCached, NULL, FALSE, nt::NormalPagePriority);
	if (!mappingStartAddress) {
		Log(L"[-] Can't set mdl pages cache, cleaning up." << std::endl);
		intel_driver::MmFreePagesFromMdl(iqvw64e_device_handle, mdl);
		intel_driver::FreePool(iqvw64e_device_handle, mdl);
		return { 0 };
	}

	const auto result = intel_driver::MmProtectMdlSystemAddress(iqvw64e_device_handle, mdl, PAGE_EXECUTE_READWRITE);
	if (!result) {
		Log(L"[-] Can't change protection for mdl pages, cleaning up" << std::endl);
		intel_driver::MmUnmapLockedPages(iqvw64e_device_handle, mappingStartAddress, mdl);
		intel_driver::MmFreePagesFromMdl(iqvw64e_device_handle, mdl);
		intel_driver::FreePool(iqvw64e_device_handle, mdl);
		return { 0 };
	}
	Log(L"[+] Allocated pages for mdl" << std::endl);

	if (mdlPtr)
		*mdlPtr = mdl;

	return mappingStartAddress;
}

uint64_t kmdf_loader2::AllocIndependentPages(HANDLE device_handle, uint32_t size)
{
	const auto base = intel_driver::MmAllocateIndependentPagesEx(device_handle, size);
	if (!base)
	{
		Log(L"[-] Error allocating independent pages" << std::endl);
		return 0;
	}

	if (!intel_driver::MmSetPageProtection(device_handle, base, size, PAGE_READWRITE)) //PAGE_EXECUTE_READWRITE
	{
		Log(L"[-] Failed to change page protections" << std::endl);
		intel_driver::MmFreeIndependentPages(device_handle, base, size);
		return 0;
	}

	return base;
}

struct driver_imports {
	uintptr_t
		IoCreateDriver,
		IofCompleteRequest,
		IoCreateSymbolicLink,
		IoCreateDevice,
		RtlInitUnicodeString,
		ZwQuerySystemInformation,
		ZwFreeVirtualMemory,
		ZwProtectVirtualMemory,
		ZwAllocateVirtualMemory,
		ZwQueryVirtualMemory,
		ExAllocatePool,
		PsLookupProcessByProcessId,
		MmCopyVirtualMemory,
		ObfDereferenceObject,
		PsGetProcessPeb,
		KeAttachProcess,
		KeDetachProcess,
		IoAllocateMdl,
		MmProbeAndLockPages,
		MmMapLockedPagesSpecifyCache,
		MmProtectMdlSystemAddress,
		MmUnmapLockedPages,
		MmUnlockPages,
		IoFreeMdl,
		IoGetCurrentProcess,
		MmCopyMemory,
		MmMapIoSpaceEx,
		MmUnmapIoSpace,
		ExFreePoolWithTag,
		MmGetPhysicalMemoryRanges,
		MmGetVirtualForPhysical,
		PsGetProcessSectionBaseAddress,
		IoGetDeviceObjectPointer,
		MmIsAddressValid,
		PsGetProcessId,
		ObReferenceObjectByName,
		IoDriverObjectType,
		RtlCreateSecurityDescriptor,
		RtlLengthSid,
		RtlCreateAcl,
		RtlAddAccessAllowedAce,
		RtlSetDaclSecurityDescriptor,
		ZwCreateSection,
		ZwClose,
		ZwMapViewOfSection,
		KeLowerIrql,
		KfRaiseIrql,
		KeDelayExecutionThread,
		SeExports,
		MmGetPhysicalAddress;
} imported;
/*
struct custom_buffer_t
{
	void* data;
	int size;
};*/

__forceinline void WriteMemoryInByteSteps(HANDLE device_handle, uintptr_t real_base, const void* local_image_data, std::size_t total_size) {
	const std::size_t step_size = 79;
	const unsigned char* data_ptr = static_cast<const unsigned char*>(local_image_data);

	for (std::size_t offset = 0; offset < total_size; offset += step_size) {
		std::size_t remaining_size = total_size - offset;
		std::size_t write_size = (remaining_size < step_size) ? remaining_size : step_size;

		intel_driver::WriteMemory(device_handle, real_base + offset, (PVOID)((uintptr_t)data_ptr + offset), write_size);
	}
}

uint64_t kmdf_loader2::MapDriver(HANDLE iqvw64e_device_handle, BYTE* data) {

	const PIMAGE_NT_HEADERS64 nt_headers = portable_executable::GetNtHeaders(data);

	if (!nt_headers) {
		Log(L"[-] Invalid format of PE image" << std::endl);
		return 0;
	}

	if (nt_headers->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		Log(L"[-] Image is not 64 bit" << std::endl);
		return 0;
	}

	uint32_t image_size = nt_headers->OptionalHeader.SizeOfImage;

	void* local_image_base = VirtualAlloc(nullptr, image_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!local_image_base)
		return 0;

	DWORD TotalVirtualHeaderSize = (IMAGE_FIRST_SECTION(nt_headers))->VirtualAddress;
	image_size = image_size - TotalVirtualHeaderSize;

	uint64_t kernel_image_base = 0;
	uint64_t mdlptr = 0;

	//kernel_image_base = intel_driver::AllocatePool(iqvw64e_device_handle, nt::POOL_TYPE::NonPagedPool, image_size);
	//kernel_image_base = AllocIndependentPages(iqvw64e_device_handle, image_size);
	//std::wstring handle_name = std::wstring(L"BBE93");

	//custom_buffer_t handle_name_buffer = {};
	//handle_name_buffer.size = (handle_name.size() * 2);

	//extend size for handle name
	//image_size += handle_name_buffer.size;

	size_t pagesToAlloc = image_size >> PAGE_2MB_SHIFT;
	pagesToAlloc += ADDRMASK_EPT_PML2_OFFSET(image_size) ? 1 : 0;

	LARGE_INTEGER maxAddress = { 0 };
	maxAddress.QuadPart = MAXULONG64;

	kernel_image_base = intel_driver::MmAllocateContiguousMemory(iqvw64e_device_handle, (pagesToAlloc + 1) * PAGE_2MB_SIZE, maxAddress);
	if (!kernel_image_base) {
		Log(L"[-] Failed to allocate remote image in kernel" << std::endl);

		VirtualFree(local_image_base, 0, MEM_RELEASE);
		return 0;
	}
	
	uint64_t physical_image_base = 0;
	intel_driver::GetPhysicalAddress(iqvw64e_device_handle, kernel_image_base, &physical_image_base);
	uint64_t offset_to_next_page = PAGE_2MB_SIZE - ADDRMASK_EPT_PML2_OFFSET(physical_image_base);
	kernel_image_base += offset_to_next_page;

	Log("[+] Relocated : 0x%llx - 0x%llx - 0x%llx\n", kernel_image_base, physical_image_base + offset_to_next_page, (pagesToAlloc + 1) * PAGE_2MB_SIZE);

	//set address for handle name
	//handle_name_buffer.data = (void*)(kernel_image_base + image_size - handle_name_buffer.size);

	do {
		Log(L"[+] Image base has been allocated at 0x" << reinterpret_cast<void*>(kernel_image_base) << std::endl);

		memcpy(local_image_base, data, nt_headers->OptionalHeader.SizeOfHeaders);

		const PIMAGE_SECTION_HEADER current_image_section = IMAGE_FIRST_SECTION(nt_headers);

		for (auto i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i) {
			if ((current_image_section[i].Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) > 0)
				continue;

			auto local_section = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(local_image_base) + current_image_section[i].VirtualAddress);
			memcpy(local_section, reinterpret_cast<void*>(reinterpret_cast<uint64_t>(data) + current_image_section[i].PointerToRawData), current_image_section[i].SizeOfRawData);

			printf("[i] copying section: %s\n", current_image_section[i].Name);

			/*if (current_image_section[i].Characteristics & IMAGE_SCN_MEM_EXECUTE)
			{
				printf("current_image_section[i].SizeOfRawData: %d\n", current_image_section[i].SizeOfRawData);
				
				int changed = 0;

				for (int ie = 0; ie < current_image_section[i].SizeOfRawData; ie++)
				{
					unsigned char* current_byte = (unsigned char*)((uintptr_t)local_section + ie);
					//printf("\n%d %x", i, *(unsigned char*)((uintptr_t)local_section + i));
					
					if (*current_byte == (unsigned char)0xCC)
					{
						unsigned char random_byte = static_cast<unsigned char>(std::rand() % CHAR_MAX);

						*current_byte = random_byte;

						changed += 1;

						//break;
					}
				}

				printf("[+] destroyed executable section (and its pages) hashes. section: %s, amout destroyed: %d\n", current_image_section[i].Name, changed);
			}*/

			if (strstr((const char*)current_image_section[i].Name, ".rdata"))
			{
				printf("[+] nulled .rdata\n");
				memset(local_section, 0, current_image_section[i].SizeOfRawData);
				//memset(local_section, 0, 0x98); //0x98 = hard code size of debug info
			}

			if (strstr((const char*)current_image_section[i].Name, ".pdata"))
			{
				printf("[+] nulled .pdata\n");
				memset(local_section, 0, current_image_section[i].SizeOfRawData);
			}
		}


		uint64_t realBase = kernel_image_base;
		kernel_image_base -= TotalVirtualHeaderSize;
		Log(L"[+] Skipped 0x" << std::hex << TotalVirtualHeaderSize << L" bytes of PE Header" << std::endl);



		RelocateImageByDelta(portable_executable::GetRelocs(local_image_base), kernel_image_base - nt_headers->OptionalHeader.ImageBase);

		if (!FixSecurityCookie(local_image_base, kernel_image_base ))
		{
			Log(L"[-] Failed to fix cookie" << std::endl);
			return 0;
		}

		if (!ResolveImports(iqvw64e_device_handle, portable_executable::GetImports(local_image_base))) {
			Log(L"[-] Failed to resolve imports" << std::endl);
			kernel_image_base = realBase;
			break;
		}


		/*if (!intel_driver::WriteMemory(iqvw64e_device_handle, realBase, (PVOID)((uintptr_t)local_image_base + TotalVirtualHeaderSize), image_size)) {
			Log(L"[-] Failed to write local image to remote image" << std::endl);
			kernel_image_base = realBase;
			break;
		}*/

		//system("pause");
		Log(L"[+] t1" << std::endl);
		WriteMemoryInByteSteps(iqvw64e_device_handle, realBase, (PVOID)((uintptr_t)local_image_base + TotalVirtualHeaderSize), image_size/* - handle_name_buffer.size*/);

		////write handle name
		//{
		//	//system("pause");
		//	Log(L"[+] t2" << std::endl);
		//	intel_driver::WriteMemory(iqvw64e_device_handle, (uintptr_t)handle_name_buffer.data, (void*)handle_name.c_str(), handle_name_buffer.size);

		//	wchar_t buffer[1337];
		//	intel_driver::ReadMemory(iqvw64e_device_handle, (uintptr_t)handle_name_buffer.data, buffer, handle_name_buffer.size);
		//	printf("[+] read drv name: \"%ws\"", buffer);

		//	//WriteMemoryInByteSteps(iqvw64e_device_handle, (uintptr_t)handle_name_buffer.data, handle_name, handle_name_buffer.size);
		//}

		/*
		for (auto i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i) {
			if ((current_image_section[i].Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) > 0)
				continue;

			bool r = current_image_section[i].Characteristics & IMAGE_SCN_MEM_READ;
			bool w = current_image_section[i].Characteristics & IMAGE_SCN_MEM_WRITE;
			bool x = current_image_section[i].Characteristics & IMAGE_SCN_MEM_EXECUTE;

			printf("[+] Section %s: %d %d %d\n", current_image_section[i].Name, r, w, x);

			int protection = 0;
			if (r && w && x)
				protection = PAGE_EXECUTE_READWRITE;
			else if (r && w)
				protection = PAGE_READWRITE;
			else if (r && x)
				protection = PAGE_EXECUTE_READ;
			else if (r)
				protection = PAGE_READONLY;
			else
				protection = PAGE_READONLY;

			if (!intel_driver::MmSetPageProtection(iqvw64e_device_handle, realBase + current_image_section[i].VirtualAddress, current_image_section[i].SizeOfRawData, protection))
			{
				Log(L"[-] Failed to change page protections another time" << std::endl);
				
				continue;
			}
		}

		system("pause");*/

		imported.IoCreateDriver = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoCreateDriver");
		imported.IofCompleteRequest = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IofCompleteRequest");
		imported.IoCreateSymbolicLink = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoCreateSymbolicLink");
		imported.IoCreateDevice = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoCreateDevice");
		imported.RtlInitUnicodeString = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "RtlInitUnicodeString");
		imported.ZwQuerySystemInformation = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwQuerySystemInformation");
		imported.ZwFreeVirtualMemory = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwFreeVirtualMemory");
		imported.ZwProtectVirtualMemory = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwProtectVirtualMemory");
		imported.ZwAllocateVirtualMemory = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwAllocateVirtualMemory");
		imported.ZwQueryVirtualMemory = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwQueryVirtualMemory");
		imported.ExAllocatePool = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ExAllocatePool");
		imported.PsLookupProcessByProcessId = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "PsLookupProcessByProcessId");
		imported.MmCopyVirtualMemory = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmCopyVirtualMemory");
		imported.ObfDereferenceObject = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ObfDereferenceObject");
		imported.PsGetProcessPeb = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "PsGetProcessPeb");
		imported.KeAttachProcess = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "KeAttachProcess");
		imported.KeDetachProcess = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "KeDetachProcess");
		imported.IoAllocateMdl = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoAllocateMdl");
		imported.MmProbeAndLockPages = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmProbeAndLockPages");
		imported.MmMapLockedPagesSpecifyCache = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmMapLockedPagesSpecifyCache");
		imported.MmProtectMdlSystemAddress = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmProtectMdlSystemAddress");
		imported.MmUnmapLockedPages = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmUnmapLockedPages");
		imported.MmUnlockPages = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmUnlockPages");
		imported.IoFreeMdl = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoFreeMdl");
		imported.IoGetCurrentProcess = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoGetCurrentProcess");
		imported.MmCopyMemory = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmCopyMemory");
		imported.MmMapIoSpaceEx = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmMapIoSpaceEx");
		imported.MmUnmapIoSpace = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmUnmapIoSpace");
		imported.ExFreePoolWithTag = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ExFreePoolWithTag");
		imported.MmGetPhysicalMemoryRanges = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmGetPhysicalMemoryRanges");
		imported.MmGetVirtualForPhysical = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmGetVirtualForPhysical");
		imported.PsGetProcessSectionBaseAddress = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "PsGetProcessSectionBaseAddress");
		imported.IoGetDeviceObjectPointer = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoGetDeviceObjectPointer");
		imported.MmIsAddressValid = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmIsAddressValid");
		imported.PsGetProcessId = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "PsGetProcessId");
		imported.ObReferenceObjectByName = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ObReferenceObjectByName");
		imported.IoDriverObjectType = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoDriverObjectType");
		imported.RtlCreateSecurityDescriptor = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "RtlCreateSecurityDescriptor");
		imported.RtlLengthSid = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "RtlLengthSid");
		imported.RtlCreateAcl = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "RtlCreateAcl");
		imported.RtlAddAccessAllowedAce = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "RtlAddAccessAllowedAce");
		imported.RtlSetDaclSecurityDescriptor = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "RtlSetDaclSecurityDescriptor");
		imported.ZwCreateSection = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwCreateSection");
		imported.ZwClose = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwClose");
		imported.ZwMapViewOfSection = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "ZwMapViewOfSection");
		imported.KeLowerIrql = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "KeLowerIrql");
		imported.KfRaiseIrql = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "KfRaiseIrql");
		imported.KeDelayExecutionThread = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "KeDelayExecutionThread");
		imported.SeExports = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "SeExports");
		imported.MmGetPhysicalAddress = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "MmGetPhysicalAddress");

		/*MouseClassServiceCallback,
		KeyboardClassServiceCallback;*/
		/*KeLowerIrql,
		KfRaiseIrql,
		KeDelayExecutionThread,
		SeExports;*/
		//printf("\nxddd %p", intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, "IoDriverObjectType"));

		const uint64_t address_of_entry_point = kernel_image_base + nt_headers->OptionalHeader.AddressOfEntryPoint;

		Log(L"[<] Calling DriverEntry 0x" << reinterpret_cast<void*>(address_of_entry_point) << std::endl);
		//system("pause");
		NTSTATUS status = 0;
		if (!intel_driver::CallKernelFunction(iqvw64e_device_handle, &status, address_of_entry_point, imported/*, handle_name_buffer*/)) {
			Log(L"[-] Failed to call driver entry" << std::endl);
			kernel_image_base = realBase;
			break;
		}

		printf("[+]driver returned 0x%x (= %d)\n", status, status);

		Log(L"[+] DriverEntry returned 0x" << std::hex << status << std::endl);
		//system("pause");
		//if free
		//intel_driver::FreePool(iqvw64e_device_handle, realBase);

		VirtualFree(local_image_base, 0, MEM_RELEASE);
		return realBase;

	} while (false);

	//failure

	VirtualFree(local_image_base, 0, MEM_RELEASE);

	Log(L"[+] Freeing memory" << std::endl);
	bool free_status = false;

	free_status = intel_driver::FreePool(iqvw64e_device_handle, kernel_image_base);
	//free_status = intel_driver::MmFreeIndependentPages(iqvw64e_device_handle, kernel_image_base, image_size);

	if (free_status) {
		Log(L"[+] Memory has been released" << std::endl);
	}
	else {
		Log(L"[-] WARNING: Failed to free memory!" << std::endl);
	}

	return 0;
}

void kmdf_loader2::RelocateImageByDelta(portable_executable::vec_relocs relocs, const uint64_t delta) {
	for (const auto& current_reloc : relocs) {
		for (auto i = 0u; i < current_reloc.count; ++i) {
			const uint16_t type = current_reloc.item[i] >> 12;
			const uint16_t offset = current_reloc.item[i] & 0xFFF;

			if (type == IMAGE_REL_BASED_DIR64)
				*reinterpret_cast<uint64_t*>(current_reloc.address + offset) += delta;
		}
	}
}

// Fix cookie by @Jerem584
bool kmdf_loader2::FixSecurityCookie(void* local_image, uint64_t kernel_image_base)
{
	auto headers = portable_executable::GetNtHeaders(local_image);
	if (!headers)
		return false;

	auto load_config_directory = headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress;
	if (!load_config_directory)
	{
		Log(L"[+] Load config directory wasn't found, probably StackCookie not defined, fix cookie skipped" << std::endl);
		return true;
	}
	
	auto load_config_struct = (PIMAGE_LOAD_CONFIG_DIRECTORY)((uintptr_t)local_image + load_config_directory);
	auto stack_cookie = load_config_struct->SecurityCookie;
	if (!stack_cookie)
	{
		Log(L"[+] StackCookie not defined, fix cookie skipped" << std::endl);
		return true; // as I said, it is not an error and we should allow that behavior
	}

	stack_cookie = stack_cookie - (uintptr_t)kernel_image_base + (uintptr_t)local_image; //since our local image is already relocated the base returned will be kernel address

	if (*(uintptr_t*)(stack_cookie) != 0x2B992DDFA232) {
		Log(L"[-] StackCookie already fixed!? this probably wrong" << std::endl);
		return false;
	}

	Log(L"[+] Fixing stack cookie" << std::endl);

	auto new_cookie = 0x2B992DDFA232 ^ GetCurrentProcessId() ^ GetCurrentThreadId(); // here we don't really care about the value of stack cookie, it will still works and produce nice result
	if (new_cookie == 0x2B992DDFA232)
		new_cookie = 0x2B992DDFA233;

	*(uintptr_t*)(stack_cookie) = new_cookie; // the _security_cookie_complement will be init by the driver itself if they use crt
	return true;
}

bool kmdf_loader2::ResolveImports(HANDLE iqvw64e_device_handle, portable_executable::vec_imports imports) {
	for (const auto& current_import : imports) {
		ULONG64 Module = utils::GetKernelModuleAddress(current_import.module_name);
		if (!Module) {
#if !defined(DISABLE_OUTPUT)
			//std::cout << "[-] Dependency " << current_import.module_name << " wasn't found" << std::endl;
#endif
			return false;
		}

		for (auto& current_function_data : current_import.function_datas) {
			uint64_t function_address = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, Module, current_function_data.name);

			if (!function_address) {
				//Lets try with ntoskrnl
				if (Module != intel_driver::ntoskrnlAddr) {
					function_address = intel_driver::GetKernelModuleExport(iqvw64e_device_handle, intel_driver::ntoskrnlAddr, current_function_data.name);
					if (!function_address) {
#if !defined(DISABLE_OUTPUT)
						//std::cout << "[-] Failed to resolve import " << current_function_data.name << " (" << current_import.module_name << ")" << std::endl;
#endif
						return false;
					}
				}
			}

			*current_function_data.address = function_address;
		}
	}

	return true;
}

#include "../Core/PS4/Loader/Elf.h"

#ifdef _WIN64
	#include <windows.h>
#else
	#include <sys/mman.h>
#endif

#include "../Util/Log.h"

namespace Memory
{
	namespace VirtualMemory {

		u64 memory_alloc(u64 address, u64 size)
		{
			//TODO it supports only execute_read_write mode
			#ifdef _WIN64
				auto ptr = reinterpret_cast<uintptr_t>(VirtualAlloc(reinterpret_cast<LPVOID>(static_cast<uintptr_t>(address)), 
																    size,
																	static_cast<DWORD>(MEM_COMMIT) | static_cast<DWORD>(MEM_RESERVE),
																	PAGE_EXECUTE_READWRITE));

				if (ptr == 0)
				{
					auto err = static_cast<u32>(GetLastError());
					LOG_ERROR_IF(true, "VirtualAlloc() failed: 0x{:X}\n", err);
				}
			#else
				auto ptr = reinterpret_cast<uintptr_t>(mmap(reinterpret_cast<void *>(static_cast<uintptr_t>(address)),
															size,
															PROT_EXEC | PROT_READ | PROT_WRITE,
															MAP_ANONYMOUS | MAP_PRIVATE,
															-1,
															0));

				if (ptr == reinterpret_cast<uintptr_t>MAP_FAILED)
				{
					LOG_ERROR_IF(true, "mmap() failed: {}\n", std::strerror(errno));
				}
			#endif
			return ptr;
		}
	}
}

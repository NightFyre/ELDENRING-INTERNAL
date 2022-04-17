#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <windowsx.h>
#include <winternl.h>
#include <psapi.h>

#include <dxgi1_4.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include <DirectXMath.h>

#include <DirectXPackedVector.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <atomic>
#include <bitset>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "Vector.hpp"
#include <MinHook.h>

extern void Nop(BYTE* dst, unsigned int size);
extern void Patch(BYTE* dst, BYTE* src, unsigned int size);

namespace ER {

	using namespace std::chrono_literals;
	using namespace DirectX;
	using namespace DirectX::PackedVector;
	namespace fs = std::filesystem;

	inline HMODULE g_Module{};
	inline std::atomic_bool g_Running = true;

	template<typename T> inline T RPM(uint64_t lpBaseAddress)
	{
		__try
		{
			T A{};
			A = *(T*)(lpBaseAddress);
			return A;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return T{};
		}
	}

	template<typename T> inline void WPM(uint64_t lpBaseAddress, T Val)
	{
		__try
		{
			*(T*)(lpBaseAddress) = Val;
			return;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	static uint64_t dwGameBase = (uint64_t)GetModuleHandle(NULL);
}

#include "Game.hpp"
#include "GameEnums.hpp"
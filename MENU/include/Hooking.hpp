#pragma once
#include "Common.hpp"

namespace ER {
#define DeclareFunction(NAME, TYPE, PARAMS, ADDRESS) TYPE(*NAME)PARAMS = (decltype(NAME))GetAddr(ADDR)
#define FunctionTop(NAME, TYPE, PARAMS, ADDRESS) TYPE(*NAME)PARAMS = reinterpret_cast<decltype(NAME))>GetAddr(ADDR)

	//reinterpret_cast<decltype(&HookInput)>(g_Hooking->m_OriginalInputHandler)(a1, a2);
	class Hooking
	{
		public:
			explicit Hooking();
			~Hooking() noexcept;
			Hooking(Hooking const&) = delete;
			Hooking(Hooking&&) = delete;
			Hooking& operator=(Hooking const&) = delete;
			Hooking& operator=(Hooking&&) = delete;

			void Hook();
			void Unhook();
			void FindHooks();

			//	Get Static Address
			__int64 GetAddr(INT64 addr);

			// Hook Pointers
			void* m_OriginalInputHandler{};
			uint64_t m_InputHandler{};

			void* m_OriginalW2SHandler{};
			INT64 m_W2SHandler{};

			INT64 TRUE_W2S = 0;
			static void __fastcall HookInput(uint64_t a1, uint64_t a2);
			static void __fastcall HookW2S(INT64 a1, INT64 a2, INT64 a3, void* a4);
	};

	inline std::unique_ptr<Hooking> g_Hooking;
}


#include "helper.hpp"

namespace ERLauncher {
	class Launcher
	{
	public:
		explicit Launcher();
		~Launcher() noexcept = default;
		Launcher(Launcher const&) = delete;
		Launcher(Launcher&&) = delete;
		Launcher& operator=(Launcher const&) = delete;
		Launcher& operator=(Launcher&&) = delete;

		void InitStyle();
		void Draw(bool* p_OPEN);
		bool createProcess(bool INJECT);
		void clearProcess(bool TERMINATE);
		bool Inject(HANDLE Process);
	};

	struct Process {
		PROCESS_INFORMATION pInfo;
		STARTUPINFO sInfo;
		const wchar_t* PATH = L"eldenring.exe";
		const wchar_t* PARAMS = L"-eac-nop-loaded";

		HANDLE hProc{};
		uintptr_t moduleBase{};
		DWORD PID{};
		DWORD exitCode{};
	};

	class LauncherVariables
	{
	public:

		Process Proc{};
		HINSTANCE hInstance{};

		//	LAUNCHER
		bool m_LauncherRunning{};
		bool m_ShowWindow{};
		bool m_DemoWindow{};
		bool m_INJECT{};

		ImVec2 BUTTONS{ 200, 30 };
		//	INIT
		explicit LauncherVariables();
		~LauncherVariables() noexcept = default;
		LauncherVariables(LauncherVariables const&) = delete;
		LauncherVariables(LauncherVariables&&) = delete;
		LauncherVariables& operator=(LauncherVariables const&) = delete;
		LauncherVariables& operator=(LauncherVariables&&) = delete;
	};

	inline std::unique_ptr<Launcher> g_Launcher;
	inline std::unique_ptr<LauncherVariables> g_LauncherVariables;
}
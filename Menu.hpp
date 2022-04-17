#pragma once
#include "Common.hpp"
#include "ImGui/imgui.h"

namespace ER {

	class Menu
	{
	public:
		//	VARIABLES
		bool	dbgPrintW2S = FALSE;
		float	dbg_crosshair_radius{};
		int		dbg_crosshair_segments{};
		float	dbg_crosshair_thickness{};
		float	dbg_crosshair_r{};
		float	dbg_crosshair_g{};
		float	dbg_crosshair_b{};
		float	dbg_crosshair_a = 1.0;

		//  FPS
		int SET_FPS = 0xE07F52;
		bool b_FPS = FALSE;
		float f_FPS = 60;
		int c_FPS = 60;

		//	CROSSHAIR COLOR
		ImVec4 color = ImVec4(0, 0, 0, 0);
		bool alpha_preview = true;
		bool alpha_half_preview = false;
		bool drag_and_drop = true;
		bool options_menu = true;
		bool hdr = false;
		ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0)
			| (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop)
			| (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0))
			| (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);


		bool m_BOXES = FALSE;
		bool debugDRAW = FALSE;
		bool m_dbgSkeleton = FALSE;
		bool m_dbgHitBoxes = FALSE;
		bool m_dbgCrosshair = FALSE;
		bool dbg_FPS = FALSE;
		bool m_dbgFreezeTarget = FALSE;
		bool bESP = FALSE;

		//  WINDOWS
		bool m_dbgEntityWnd = FALSE;
		bool m_dbgMatrixWnd = FALSE;

		//  
		float Matrix[16];
		float ViewMatrix[16];

		//  Skeleton Draw
		bool s_draw = FALSE;
		float s_drawDistance = 0;

		//  FREEZE ENTITIES
		bool f_TOGGLE = FALSE;
		Vector3 storedPOS[999];     //  trouble

		//  DISTANCE FREEZE
		uintptr_t ents[30];         // ENTITY OBJECT
		Vector3 distFREEZE[30];     // STORED COORDS


		//	INITIALIZE CLASS
		Menu()  noexcept = default;
		~Menu() noexcept = default;
		Menu(Menu const&) = delete;
		Menu(Menu&&) = delete;
		Menu& operator=(Menu const&) = delete;
		Menu& operator=(Menu&&) = delete;

		//	FORWARD DECLARE FUNCTIONS
		void InitStyle();
		void Draw();


		void AboutTab();
		void CharacterStats();
		void EntityStats();
		void Visuals();
		void DebugOptions();
		
	private:
		bool m_StyleInit{};
	};

	inline std::unique_ptr<Menu> g_Menu;

};
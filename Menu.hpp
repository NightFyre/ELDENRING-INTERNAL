#pragma once
#include "Common.hpp"
#include "ImGui/imgui.h"
//	CLEANING IS REQUIRED
namespace ER {

	struct TextPositions {
		ImVec2 posONE = { 20, 5 };
		ImVec2 posTWO = { 20, 15 };
		ImVec2 posTHREE = { 20, 25 };
		ImVec2 posFIVE = { 20, 35 };
		ImVec2 posSIX = { 20, 45 };
		ImVec2 posSEVEN = { 20, 55 };
		ImVec2 posEIGHT = { 20, 65 };
		ImVec2 posNINE = { 20, 75 };
		ImVec2 posTEN = { 20, 85 };
	};

	class Menu
	{
	public:

		///	OFFSETS
		//	QWORDS
		int ptr_DEBUG_FLAGS = 0x03C0C1C8;	//	DEBUG FLAGS
		int ptr_NBOTT_W2S = 0x03C04828;		//	NBOTT WorldToScreen qword_ptr
		int ptr_PLAYER_DATA = 0x39DAD60;	//	Player Data qword_ptr
		
		///	VARIABLES
		//	MENU SELECTION FLAGS
		bool debugDRAW = FALSE;			//	ESP
		bool m_dbgSkeleton = FALSE;		//	ESP Skeleton
		bool m_dbgHitBoxes = FALSE;		//	Debug Hitbox Flag
		bool m_dbgCrosshair = FALSE;	//	Crosshair Flag
		bool m_RGB_CROSSHAIR = FALSE;	//	RGB Crosshair Flag
		bool dbg_FPS = FALSE;			//	FPS Flag
		bool m_dbgFreezeTarget = FALSE;	//	Freeze Target Flag
		bool bESP = FALSE;				//	ESP

		//	STATIC
		int ptr_SET_FPS = 0xDE8B72;			// FPS	|	AOB + 0x3	|	OLD = E07F4F + 0x3 = 0xE07F52
		int ptr_W2S_FUNCTION = 0x07527A0;	// 0x0752890;	//	W2S Function

		//	RAINBOW THEME
		ImColor dbg_RAINBOW{};			//	RAINBOW THEME COLORS
		bool dbg_RAINBOW_THEME{};		//	RAINBOW THEME BOOLEAN

		bool	dbg_ENT_RGB = FALSE;
		bool	dbgPrintW2S = FALSE;	//	???
		
		//	CROSSHAIR OPTIONS
		float	dbg_crosshair_radius{};
		int		dbg_crosshair_segments{};
		float	dbg_crosshair_thickness{};
		ImColor dbg_crosshair_color{};
		float	dbg_crosshair_a = 1.0;

		//  FPS
		bool b_FPS = FALSE;
		float f_FPS = 60;
		int c_FPS = 60;

		//  WINDOWS
		bool m_dbgEntityWnd = FALSE;
		bool m_dbgMatrixWnd = FALSE;

		//  ESP Array
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

		//	CROSSHAIR COLOR SELECTION
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

		TextPositions PrintToScreen{};

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
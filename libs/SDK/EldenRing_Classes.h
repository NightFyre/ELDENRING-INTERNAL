#pragma once

/**
 * Name: Elden Ring
 * Version: ~
 * Engine: ~
*/

#pragma pack(push, 0x01)
namespace HEXINTON
{
	// Template class for obtaining globals
	class CGlobals
	{
	public:
		static class GameManager**				GGameMan;
		static class GameDataManager**			GGameDataMan;
		static class WorldCharacterManager**	GWorldCharMan;
	};

	class GameManager
	{
	private:
		char									pad_0000[8];			//	0x0000
		ChrGameData*							pChrGameData;			//	0x0008
	};

	class GameDataManager
	{
	private:

	};

	class WorldCharacterManager
	{
	private:	
		char								pad_0008[69360];			//0x0008
		class LocalPlayer*					pLocalPlayer;				//0x10EF8
		char								pad_10F00[58032];			//0x10F00
		uint64_t							pEntArrayBegin;				//0x1F1B0
		uint64_t							pEntArrayEnd;				//0x1F1B8
	
	private:
		virtual void						vf_Function0();

	public:
		class LocalPlayer*					GetLocalPlayer();
		class PlayerInstance*				GetLocalPlayerInstance();
		class CharacterModules*				GetLocalPlayerCharModules();
		class CSCharData*					GetLocalPlayerCharData();
		class CSCharPhysics*				GetLocalPlayerCharPhysics();
		bool								GetCharacterArray(std::vector<PlayerInstance*>* pOut);
	};	//Size: 0x10F00

	class LocalPlayer
	{
	private:
		class PlayerInstance*				pPlayer;					//0x0000
		int32_t								dwflag;						//0x0008

	public:
		PlayerInstance*						GetPlayerInstance();
	};	//Size: 0x000C

	class PlayerInstance
	{
	private:
		int32_t								HandleID;					//0x0008
		char								pad_000C[4];				//0x000C
		class LocalPlayer*					pLocalPlayer;				//0x0010
		char								pad_0018[16];				//0x0018
		class CharRes*						pCharRes;					//0x0028
		char								pad_0030[32];				//0x0030
		class CSCharModelIns*				pCharModelInstance;			//0x0050
		class CSPlayerCtrl*					pPlayerCtrl;				//0x0058
		int32_t								paramID;					//0x0060
		char								pad_0064[4];				//0x0064
		int32_t								CharacterType;				//0x0068
		int32_t								TeamType;					//0x006C
		char								pad_0070[80];				//0x0070
		class CSTargetVelocityRecorder*		pTargetVelocityRecorder;	//0x00C0
		char								pad_00C8[40];				//0x00C8
		class PlayerInstance*				pPlayerInstance;			//0x00F0
		char								pad_00F8[128];				//0x00F8
		class CSSpecialEffect*				pSpecialEffects;			//0x0178
		char								pad_0180[8];				//0x0180
		int32_t								CharacterID;				//0x0188
		char								pad_018C[4];				//0x018C
		class CharacterModules*				pCharModules;				//0x0190


	private:
		virtual void						vf_Function0();

	public:
		CharacterModules*					GetCharacterModules();
		int32_t								GetCharType();
		int32_t								GetTeam();
		void								SetCharType(const int32_t newType);
		void								SetTeam(const int32_t newTeam);
		bool								GetHP(int32_t& outHealth);
		bool								GetPos(Vector3& outPos);
		float								GetDistance(const Vector3 Other);
		void								SetHP(const int32_t value);
		void								SetPos(const Vector3 In);
	};	//Size: 0x0198

	class CharacterModules
	{
	private:
		class CSCharData*					pCharData;					//0x0000
		class CSCharActionFlags*			pCharActionFlags;			//0x0008
		class CSCharBehaviorScripts*		pCharBehavScripts;			//0x0010
		class CSCharTimeAct*				pCharTimeAct;				//0x0018
		class CSCharResist*					pCharResist;				//0x0020
		class CSCharBehavior*				pCharBehavior;				//0x0028
		class CSCharBehaviorSync*			pCharBehaviorSync;			//0x0030
		class CSCharAi*						pCharAI;					//0x0038
		class CSPlayerSuperArmor*			pCharSuperArmor;			//0x0040
		class CSPlayerToughness*			pPlayerToughness;			//0x0048
		class CSCharTalk*					pCharTalk;					//0x0050
		class CSCharEvent*					pCharEvent;					//0x0058
		class CSPlayerMagic*				pPlayerMagic;				//0x0060
		class CSCharPhysics*				pCharPhysics;				//0x0068
		class CSCharFall*					pCharFall;					//0x0070
		class CSPlayerLadder*				PlayerLadder;				//0x0078
		class CSCharActionRequest*			pCharActionRequest;			//0x0080
		class CSCharThrow*					pCharThrow;					//0x0088
		class CSCharHitStop*				pCharHitStop;				//0x0090
		class CSPlayerDamage*				pPlayerDamage;				//0x0098

	public:
		CSCharData*							GetCharData();
		CSCharPhysics*						GetCharPhysics();
		CSPlayerDamage*						GetPlayerDamage();
	};	//Size: 0x00A0
	
	class CSCharData
	{
	private:
		char								pad_0008[304];				//0x0008
		int32_t								CurrentHealth;				//0x0138
		int32_t								N000035E3;					//0x013C
		int32_t								MaxHealth;					//0x0140
		int32_t								BaseMaxHealth;				//0x0144
		int32_t								CurrentMana;				//0x0148
		int32_t								MaxMana;					//0x014C
		int32_t								BaseMaxMana;				//0x0150
		int32_t								CurrentStamina;				//0x0154
		int32_t								MaxStamina;					//0x0158
		int32_t								BaseMaxStamina;				//0x015C

	private:
		virtual void Function0();

	public:
		int32_t								GetHealth();
		int32_t								GetMaxHealth();
		void								SetHealth(const int32_t& value);
		int32_t								GetMana();
		int32_t								GetMaxMana();
		void								SetMana(const int32_t& value);
		int32_t								GetStamina();
		int32_t								GetMaxStamina();
		void								SetStamina(const int32_t& value);
	};	//Size: 0x0160

	class CSCharPhysics
	{
	private:
		class PlayerInstance*				pPlayerInstance;			//0x0008
		class PlayerInstance*				pPlayerInstance2;			//0x0010
		class CSCharPhysics*				pCharPhysics;				//0x0018
		class CSCharData*					pCharData;					//0x0020
		class hknpCapsuleShape*				pHKCapsule;					//0x0028
		char								pad_0030[36];				//0x0030
		float								sinAzimuth;					//0x0054
		char								pad_0058[4];				//0x0058
		float								cosAzimuth;					//0x005C
		char								pad_0060[16];				//0x0060
		Vector3								RelativePosition;			//0x0070
		char								pad_007C[343];				//0x007C
		bool								bNoGravity;					//0x01D3

	private:
		virtual void						vf_Function0();

	public:
		Vector3								GetLocalPosition();
		void								SetLocalPosition(Vector3 newPosition);
		void								ToggleGravity();
	};	//Size: 0x01D4
}
#pragma pack(pop)
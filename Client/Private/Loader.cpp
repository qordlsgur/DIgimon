#include "Loader.h"

#include "BackGround.h"

#pragma region UI
#include "Digivice.h"
#include "Digivice_Slot.h"
#include "Digivice_Mask.h"
#include "Digivice_Info.h"
#include "Digivice_Skill.h"
#include "Digivice_Skill_Info.h"
#include "Digivice_Target.h"
#include "Digivice_Hp.h"
#include "Digivice_Sp.h"
#include "Digivice_Exp.h"
#include "PartyUHD.h"
#include "UHD_Slot.h"
#include "Interaction.h"
#include "Battle.h"
#include "Skill_Model.h"
#include "Skill_Effect_Image.h"
#pragma endregion

#pragma region Battle
#include "Battle_Timeline.h"
#include "Battle_Turn.h"
#include "Battle_Enemy_Hp_BG.h"
#include "Battle_Enemy_HpBar.h"
#include "Battle_Skill.h"
#include "KeyBord.h"
#pragma endregion

#pragma region Skill
#include "AngewomonSkill1.h"
#include "AngewomonSkill1_Part1.h"
#include "AngewomonSkill1_Part2.h"
#include "AngewomonSkill2.h"
#include "AngewomonSkill2_Part1.h"
#include "AngewomonSkill2_Part2.h"
#include "AngewomonSkill2_Part3.h"
#include "AngewomonSkill2_Part4.h"
#include "AngewomonSkill3.h"
#include "AngewomonSkill3_Part1.h"
#include "AngewomonSkill3_Part2.h"

#include "BlackwargreymonSkill1.h"
#include "BlackwargreymonSkill2.h"
#include "BlackwargreymonSkill3.h"
#include "BlackwargreymonSkill3_Part1.h"

#include "DevilmonSkill1.h"
#include "DevilmonSkill2.h"
#include "DevilmonSkill3.h"
#include "DevilmonSkill3_Part1.h"
#include "DevilmonSkill3_Part2.h"
#include "DevilmonSkill3_Part3.h"
#include "DevilmonSkill3_Part4.h"

#include "LadydevimonSkill1.h"
#include "LadydevimonSkill2.h"
#include "LadydevimonSkill3.h"

#include "LeomonSkill1.h"
#include "LeomonSkill2.h"
#include "LeomonSkill2_Part1.h"
#include "LeomonSkill3.h"

#include "MetalgarumonSkill1.h"
#include "MetalgarumonSkill2.h"
#include "MetalgarumonSkill2_Part1.h"
#include "MetalgarumonSkill3.h"
#include "MetalgarumonSkill3_Part1.h"

#include "MetalgreymonSkill1.h"
#include "MetalgreymonSkill1_Part1.h"
#include "MetalgreymonSkill1_Part2.h"
#include "MetalgreymonSkill1_Part3.h"
#include "MetalgreymonSkill2.h"
#include "MetalgreymonSkill2_Part1.h"
#include "MetalgreymonSkill2_Part2.h"
#include "MetalgreymonSkill2_Part3.h"
#include "MetalgreymonSkill2_Part4.h"
#include "MetalgreymonSkill3.h"
#include "MetalgreymonSkill3_Part1.h"
#include "MetalgreymonSkill3_Part2.h"
#include "MetalgreymonSkill3_Part3.h"

#include "OmegamonSkill1.h"
#include "OmegamonSkill2.h"
#include "OmegamonSkill3.h"

#include "WargreymonSkill1.h"
#include "WargreymonSkill2.h"
#include "WargreymonSkill3.h"

#include "BeelzebumonSkill1.h"
#include "BeelzebumonSkill2.h"
#include "BeelzebumonSkill3.h"
#pragma endregion

#pragma region Button
#include "Exit_Button.h"
#pragma endregion

#pragma region Map
#include "Terrain.h"
#include "BldA.h"
#include "BldB.h"
#include "BldC.h"
#include "BldD.h"
#include "BldE.h"
#include "BldH.h"
#include "BldJ.h"
#include "BldK.h"
#include "Tennis.h"
#include "Spawner.h"
#include "Tribune.h"
#include "TennisLight.h"
#include "TennisField.h"
#pragma endregion

#pragma region Digimon
#include "Player.h"
#include "Body_Player.h"
#include "Angewomon.h"
#include "Body_Angewomon.h"
#include "Omegamon.h"
#include "Body_Omegamon.h"
#include "Devilmon.h"
#include "Body_Devilmon.h"
#include "Ladydevimon.h"	
#include "Body_Ladydevimon.h"
#include "Leomon.h"
#include "Body_Leomon.h"
#include "Metalgarumon.h"
#include "Body_Metalgarumon.h"
#include "Metalgreymon.h"
#include "Body_Metalgreymon.h"
#include "Wargreymon.h"
#include "Body_Wargreymon.h"
#include "Blackwargreymon.h"
#include "Body_Blackwargreymon.h"
#include "Beelzebumon.h"
#include "Body_Beelzebumon.h"

#pragma endregion

//#include "Effect.h"
#include "Battle_Terrain.h"
#include "Monster.h"
#include "Sky.h"
#include "Navigation.h"

#include "GameInstance.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 세마포어, 뮤텍스, 크리티컬섹션 */

	/* 임계영역(힙, 데이터, 코드)에 접근하기위한 키를 생성한다. */
	InitializeCriticalSection(&m_CriticalSection);

	/* 실제 로딩을 수행하기위한 스레드를 생성한다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	CoInitializeEx(nullptr, 0);

	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;
	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void CLoader::Output()
{
	SetWindowText(g_hWnd, m_strMessage.c_str());
}

HRESULT CLoader::Loading_For_Logo()
{
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 1))))
		return E_FAIL;

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
#pragma  region TEXTURE
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");

	//디지바이스------------------------------------------------------------------------------------------------------------------------------------------------------

	/* For.Prototype_Component_Texture_DigiVice*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Digivice.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Slot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Digivice_Slot.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/DIgivice_Mask.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Target*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Target"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Digivice_Target.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Info*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Info"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Digivice_Info.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Status_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digivice/Digivice_Status_Gauge_BG.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Status_HP*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digivice/Digivice_Status_Gauge_HP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Status_SP*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_SP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digivice/Digivice_Status_Gauge_SP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Status_EXP*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Status_EXP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digivice/Digivice_Status_Gauge_EXP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digivice_Skill*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Digivice_Skill.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skill_Info*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skill_Info"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Skill_Info.png"), 1))))
		return E_FAIL;

	//Party_HUD---------------------------------------------------------------------------------------------------------------------------------------------------------

		/* For.Prototype_Component_Texture_HUD_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HUD/BattleDigimon_Mask.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HUD */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HUD/Mask.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HUD_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HUD/BattleDigimon_Frame.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HUD_SP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_SP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HUD/BattleDigimon_SP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HUD_HP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HUD/BattleDigimon_HP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HUD_EVP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_HUD_EVP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/HUD/BattleDigimon_EVP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Exit_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Exit_Button"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Exit_Button.png"), 1))))
		return E_FAIL;

	//배틀UI------------------------------------------------------------------------------------------------------------------------------------------------------

	/* For.Prototype_Component_Texture_Battle_Skill*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Skill.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Skill_Select*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Skill_Select"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Select_Skill.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Digimon_Field_HP*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Digimon_Field_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Digimon_Field_HP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digimon_Field_HP_Allies*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_Field_HP_Allies"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Digimon_Field_HP_Allies.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Digimon_Field_HP_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Digimon_Field_HP_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Digimon_Field_HP_BG.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Rotation_Gauge_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Rotation_Gauge_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Rotation_Gauge_BG.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Rotation_Gauge_HP*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Rotation_Gauge_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Rotation_Gauge_HP.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Hp_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Hp_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Battle_Hp_BG.png"), 1))))
		return E_FAIL;

	//베틀 UI----------------------------------------------------------------------------------------------------------------------------------------------

	/* For.Prototype_Component_Texture_Timeline_Portrait_mask*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Timeline_Portrait_mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Timeline_Portrait_mask.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Timeline_blue_bg*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Timeline_bg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Timeline_bg%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Timeline_blue_Line*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Timeline_Line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Timeline_Line%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Battle_Turn*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Battle_Turn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Battle/Turn.png"), 1))))
		return E_FAIL;

	//키보드 자판----------------------------------------------------------------------------------------------------------------------------------------------

	/* For.Prototype_Component_Texture_Select_Key*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Select_Key"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/KeyBord/Select_Key.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skill_KeyBord*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Skill_KeyBord"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/KeyBord/Skill%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_KeyBord_Number*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_KeyBord_Number"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/KeyBord/Key%d.png"), 10))))
		return E_FAIL;

	//디지몬UI이미지-------------------------------------------------------------------------------------------------------------------------------------------


#pragma region UI

		/* For.Prototype_Component_Texture_Digimon_HUD */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_HUD"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digimon_UI/A/A%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digimon_Digivice */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_Digivice"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digimon_UI/B/B%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Digimon_Digidex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digimon_Digidex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Digimon_UI/C/C%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Angewomon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Angewomon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Angewomon/Angewomon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Beelzebumon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Beelzebumon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Beelzebumon/Beelzebumon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Blackwargreymon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Blackwargreymon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Blackwargreymon/Blackwargreymon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Devilmon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Devilmon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Devilmon/Devilmon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Ladydevimon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Ladydevimon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Ladydevimon/Ladydevimon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Leomon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Leomon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Leomon/Leomon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Metalgarumon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Metalgarumon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgarumon/Metalgarumon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Metalgreymon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Metalgreymon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgreymon/Metalgreymon_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Omegamon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Omegamon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Omegamon/Omegamon_Tri_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Wargreymon_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Wargreymon_Skill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Wargreymon/Wargreymon_%d.png"), 3))))
		return E_FAIL;

#pragma endregion

#pragma region Default;


	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 1))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Effect */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Effect"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;

#pragma  endregion

#pragma region EffectImage

		/* For.Prototype_Component_Texture_Fire4x4 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Fire"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Fire%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Whirlwind */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Whirlwind"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Whirlwind%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Noise%d.png"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Light*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Light"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/LightEffect%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CircleEffect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CircleEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/CircleEffect%d.png"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Circle*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Circle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Circle%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Wave*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Wave"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Wave%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Space*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Space"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Space%d.png"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Smoke*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Smoke%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Spark*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Spark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Spark%d.png"), 5))))
		return E_FAIL;




#pragma endregion


#pragma region Navigation

	m_strMessage = TEXT("네비게이션를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.bin")))))
		return E_FAIL;

#pragma endregion

#pragma region MODEL

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Terrain.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Battle_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Battle_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Battle_Terrain.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//-----------------------------------------------------------------------------------------------------------------------

	_matrix			PreTransformMatrix = XMMatrixIdentity();

	// 디지몬

	/* For.Prototype_Component_Model_Player */
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Player/Player.bin", PreTransformMatrix))))
		return E_FAIL;


	//-----------------------------------------------------------------------------------------------------------------------------------

	// 오브젝트

	//	/* For.Prototype_Component_Model_Tennis */
	//PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, 1.f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Tennis"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Tennis/Tennis.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldA */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldA"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldA.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldB */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldB"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldB.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldC */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldC"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldC.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldD */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldD"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldD.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldE */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldE"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldE.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldH */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldH"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldH.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldJ */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldJ"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldJ.Bin", PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_BldK */
	//PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, -0.9f, 0.f);
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_BldK"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Bilding/BldK.Bin", PreTransformMatrix))))
	//	return E_FAIL;


	/* For.Prototype_Component_Model_TennisFloolight */
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) ;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Tennislight"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,"../Bin/Resources/Models/BattleField/TennisFloolight.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_tribune */
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(0.f, 0.f, 3.f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_tribune"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/tribune/tribune.bin", PreTransformMatrix))))
		return E_FAIL;

#pragma endregion

#pragma region SHADER

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_Shader_Image_effect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Image_effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Image_effect.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Battle_Timeline*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Battle_Timeline"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Battle_Timeline.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Battle_Enemy_Hp*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Battle_Enemy_Hp"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Battle_Enemy_Hp.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Inventory */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Inventory.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Hover */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Hover"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Hover.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_UHD */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_UHD"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_UHD.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Digivice */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Digivice.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Digivice_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_Mask"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Digivice_Mask.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Digivice_Target*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_Target"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Digivice_Target.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Digivice_State*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_State"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Digivice_State.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Skill_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Skill_Image"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Skill_Image.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Skill_Hover*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Skill_Hover"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Skill_Hover.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxSkillMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkillMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

#pragma endregion

#pragma region COLLIER

	m_strMessage = TEXT("콜라이더를(을) 로딩 중 입니다.");

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region PROTOTYPE

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Terrain"),
		CBattle_Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Interaction*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Interaction"),
		CInteraction::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Battle*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle"),
		CBattle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//디지바이스---------------------------------------------------------------------------------------------------------------------------------------------------------

	/* For.Prototype_GameObject_Digivice */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice"),
		CDigivice::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Slot"),
		CDigivice_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Mask"),
		CDigivice_Mask::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//* For.Prototype_GameObject_PartyUHD */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PartyUHD"),
		CPartyUHD::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//* For.Prototype_GameObject_UHDSlot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UHDSlot"),
		CUHD_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Info */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Info"),
		CDigivice_Info::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Skill"),
		CDigivice_Skill::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Digivice_Skill_Info */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Skill_Info"),
		CDigivice_Skill_Info::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Target */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Target"),
		CDigivice_Target::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Hp*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Hp"),
		CDigivice_Hp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Sp*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Sp"),
		CDigivice_Sp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Digivice_Exp*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Exp"),
		CDigivice_Exp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//* For.Prototype_GameObject_Exit_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Exit_Button"),
		CExit_Button::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//배틀UI----------------------------------------------------------------------------------------------------------------------------

	/* For.Prototype_GameObject_Battle_Timeline */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Timeline"),
		CBattle_Timeline::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Battle_Turn*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Turn"),
		CBattle_Turn::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Battle_Enemy_HpBG*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Enemy_HpBG"),
		CBattle_Enemy_Hp_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Battle_Enemy_Hp*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Enemy_Hp"),
		CBattle_Enemy_HpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Battle_Skill*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Skill"),
		CBattle_Skill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Battle_Skill_KayBord*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Battle_Skill_KayBord"),
		CKeyBord::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//디지몬------------------------------------------------------------------------------------------------------------------------------

		/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Player*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma region Angewomon

	/* For.Prototype_Component_Shader_VtxSkillAngewomon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillAngewomon"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkillAngewomon.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Angewomon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Angewomon/Angewomon.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_GameObject_Angewomon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Angewomon"),
		CAngewomon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Angewomon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Angewomon"),
		CBody_Angewomon::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_AngewomonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill1"),
		CAngewomonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill1_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill1_Part1"),
		CAngewomonSkill1_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill1_Part2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill1_Part2"),
		CAngewomonSkill1_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_AngewomonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill2"),
		CAngewomonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Angewomon_Skill2_1*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon2_1.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Angewomon_Skill2_2*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon2_2.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Angewomon_Skill2_3*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_3"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon2_3.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Angewomon_Skill2_4*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_4"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon2_4.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_AngewomonSkill2_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Angewomon/AngewomonSkill2_%d.png"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill2_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill2_Part1"),
		CAngewomonSkill2_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill2_Part2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill2_Part2"),
		CAngewomonSkill2_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill2_Part3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill2_Part3"),
		CAngewomonSkill2_Part3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill2_Part4*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill2_Part4"),
		CAngewomonSkill2_Part4::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_AngewomonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill3"),
		CAngewomonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AngewomonSkill3_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill3_Part1"),
		CAngewomonSkill3_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Angewomon_Skill3_1*/
	PreTransformMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixTranslation(0.f, 5.f, -5.f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill3_1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon3_0.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_GameObject_AngewomonSkill3_Part2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AngewomonSkill3_Part2"),
		CAngewomonSkill3_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Angewomon_Skill3_2*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill3_2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon3_1.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Angewomon_Skill3_3*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill3_3"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Angewomon/Angewomon3_2.bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_AngewomonSkill3_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill3_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Angewomon/AngewomonSkill3_%d.png"), 3))))
		return E_FAIL;

#pragma endregion

#pragma region Blackwargreymon
	/* For.Prototype_Component_Shader_VtxSkillBlackwargreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillBlackwargreymon"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkillBlackwargreymon.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_GameObject_Blackwargreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Blackwargreymon"),
		CBlackwargreymon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Blackwargreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Blackwargreymon"),
		CBody_Blackwargreymon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BlackwargreymonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill1"),
		CBlackwargreymonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BlackwargreymonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill2"),
		CBlackwargreymonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BlackwargreymonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill3"),
		CBlackwargreymonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_BlackwargreymonSkill3_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BlackwargreymonSkill3_Part1"),
		CBlackwargreymonSkill3_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Blackwargreymon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Blackwargreymon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Blackwargreymon/Blackwargreymon.bin", PreTransformMatrix))))
		return E_FAIL;


#pragma endregion

#pragma region Devilmon
	/* For.Prototype_Component_Shader_VtxSkillDevilmon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillDevilmon"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkillDevilmon.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxEffectDevilmon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxEffectDevilmon"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxEffectDevilmon.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_GameObject_Devilmon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Devilmon"),
		CDevilmon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Devilmon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Devilmon"),
		CBody_Devilmon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DevilmonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill1"),
		CDevilmonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DevilmonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill2"),
		CDevilmonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_DevilmonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3"),
		CDevilmonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_DevilmonSkill3_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part1"),
		CDevilmonSkill3_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_DevilmonSkill3_Part2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part2"),
		CDevilmonSkill3_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_DevilmonSkill3_Part3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part3"),
		CDevilmonSkill3_Part3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_DevilmonSkill3_Part4*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DevilmonSkill3_Part4"),
		CDevilmonSkill3_Part4::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Devilmon_Skill1*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Devilmon_Skill1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Devilmon/DevilmonSkill1.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Devilmon_Skill2*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Devilmon_Skill2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Devilmon/DevilmonSkill2.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_DevilmonSkill3_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_DevilmonSkill3_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Devilmon/Devilmon_Skill3_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Devilmon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Devilmon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Devilmon/Devilmon.bin", PreTransformMatrix))))
		return E_FAIL;


#pragma endregion

#pragma region Ladydevimon
	/* For.Prototype_GameObject_Ladydevimon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Ladydevimon"),
		CLadydevimon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Ladydevimon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Ladydevimon"),
		CBody_Ladydevimon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LadydevimonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LadydevimonSkill1"),
		CLadydevimonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LadydevimonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LadydevimonSkill2"),
		CLadydevimonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LadydevimonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LadydevimonSkill3"),
		CLadydevimonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Ladydevimon*/
	PreTransformMatrix = XMMatrixScaling(0.06f, 0.06f, 0.06f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Ladydevimon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Ladydevimon/Ladydevimon.bin", PreTransformMatrix))))
		return E_FAIL;


#pragma endregion

#pragma region Leomon
	/* For.Prototype_Component_Shader_VtxSkillLeomon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillLeomon"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkillLeomon.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_GameObject_Leomon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Leomon"),
		CLeomon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Omegamon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Leomon"),
		CBody_Leomon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LeomonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill1"),
		CLeomonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_LeomonSkill2_Part1 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill2_Part1"),
		CLeomonSkill2_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Leomon_Skill2*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Leomon_Skill2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Leomon/LeomonSkill1.bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_LeomonSkill2_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_LeomonSkill2_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Leomon/LeomonSkill2_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_LeomonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill2"),
		CLeomonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LeomonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_LeomonSkill3"),
		CLeomonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Leomon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Leomon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Leomon/Leomon.bin", PreTransformMatrix))))
		return E_FAIL;


#pragma endregion

#pragma region Metalgarumon
	/*For.Prototype_Component_Shader_VtxSkillMetalgarumon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillMetalgarumon"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkillMetalgarumon.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_GameObject_Metalgarumon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Metalgarumon"),
		CMetalgarumon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Metalgarumon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Metalgarumon"),
		CBody_Metalgarumon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalgarumonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill1"),
		CMetalgarumonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalgarumonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2"),
		CMetalgarumonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgarumonSkill2_Part1 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill2_Part1"),
		CMetalgarumonSkill2_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	/* For.Prototype_Component_Model_Metalgarumon_Skill2*/
	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Metalgarumon_Skill2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Metalgarumon/MetalGarumonSkill1.Bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_MetalgarumonSkill2_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgarumonSkill2_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgarumon/missile_diffuse.png"), 1))))
		return E_FAIL;


	/* For.Prototype_GameObject_MetalgarumonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill3"),
		CMetalgarumonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgarumonSkill3_Part1 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgarumonSkill3_Part1"),
		CMetalgarumonSkill3_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Metalgarumon_Skill3*/
	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Metalgarumon_Skill3"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Metalgarumon/MetalGarumonSkill3.Bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_MetalgarumonSkill3_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgarumonSkill3_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgarumon/MatalgarumonSkill3_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Metalgarumon*/
	PreTransformMatrix = XMMatrixScaling(0.06f, 0.06f, 0.06f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Metalgarumon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Metalgarumon/Metalgarumon.bin", PreTransformMatrix))))
		return E_FAIL;


#pragma endregion
	
#pragma region Metalgreymon
	/* For.Prototype_GameObject_Metalgreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Metalgreymon"),
		CMetalgreymon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Metalgreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Metalgreymon"),
		CBody_Metalgreymon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalgreymonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1"),
		CMetalgreymonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	/* For.Prototype_GameObject_MetalgreymonSkill1_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1_Part1"),
		CMetalgreymonSkill1_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill1_Part2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1_Part2"),
		CMetalgreymonSkill1_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill1_Part3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill1_Part3"),
		CMetalgreymonSkill1_Part3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_MetalgreymonSkill_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgreymon/MetalgreymonSkill_Image%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalgreymonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2"),
		CMetalgreymonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	/* For.Prototype_GameObject_MetalgreymonSkill2_Part1 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part1"),
		CMetalgreymonSkill2_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill2_Part2 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part2"),
		CMetalgreymonSkill2_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	/* For.Prototype_GameObject_MetalgreymonSkill2_Part3 */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part3"),
		CMetalgreymonSkill2_Part3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill2_Part4*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill2_Part4"),
		CMetalgreymonSkill2_Part4::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_MetalgreymonSkill2 */
	PreTransformMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_MetalgreymonSkill2"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Metalgreymon/MetalgreymonSkill1.Bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_MetalgreymonSkill2_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill2_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgreymon/missile.png"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_MetalgreymonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3"),
		CMetalgreymonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill3_Part1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3_Part1"),
		CMetalgreymonSkill3_Part1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill3_Part2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3_Part2"),
		CMetalgreymonSkill3_Part2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_MetalgreymonSkill3_Part3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MetalgreymonSkill3_Part3"),
		CMetalgreymonSkill3_Part3::Create(m_pDevice, m_pContext))))
		return E_FAIL;
		/* For.Prototype_Component_Texture_MetalgreymonSkill3_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MetalgreymonSkill3_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Metalgreymon/MetalgreymonSkill3_Image%d.png"), 3))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Metalgreymon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Metalgreymon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Metalgreymon/Metalgreymon.bin", PreTransformMatrix))))
		return E_FAIL;



#pragma endregion

#pragma region Omegamon
	/* For.Prototype_GameObject_Omegamon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Omegamon"),
		COmegamon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Omegamon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Omegamon"),
		CBody_Omegamon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_OmegamonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_OmegamonSkill1"),
		COmegamonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_OmegamonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_OmegamonSkill2"),
		COmegamonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_OmegamonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_OmegamonSkill3"),
		COmegamonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Omegamon*/
	PreTransformMatrix = XMMatrixScaling(0.07f, 0.07f, 0.07f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Omegamon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Omegamon/Omegamon.bin", PreTransformMatrix))))
		return E_FAIL;

#pragma endregion

#pragma region Wargreymon
	/* For.Prototype_GameObject_Wargreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Wargreymon"),
		CWargreymon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Wargreymon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Wargreymon"),
		CBody_Wargreymon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_WargreymonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WargreymonSkill1"),
		CWargreymonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_WargreymonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WargreymonSkill2"),
		CWargreymonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_Model_WargreymonSkill2_1*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_WargreymonSkill2_1"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Wargreymon/Wargreymon2_1.Bin", PreTransformMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_WargreymonSkill2_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_WargreymonSkill2_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Wargreymon/WargreymonSkill2_%d.png"), 1))))
		return E_FAIL;


	/* For.Prototype_GameObject_WargreymonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_WargreymonSkill3"),
		CWargreymonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Wargreymon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Wargreymon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Wargreymon/Wargreymon.bin", PreTransformMatrix))))
		return E_FAIL;

#pragma endregion

#pragma region Beelzebumon
	/* For.Prototype_GameObject_Beelzebumon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Beelzebumon"),
		CBeelzebumon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Beelzebumon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Beelzebumon"),
		CBody_Beelzebumon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BeelzebumonSkill1*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BeelzebumonSkill1"),
		CBeelzebumonSkill1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BeelzebumonSkill2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BeelzebumonSkill2"),
		CBeelzebumonSkill2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BeelzebumonSkill3*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BeelzebumonSkill3"),
		CBeelzebumonSkill3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Beelzebumon*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Beelzebumon"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Beelzebumon/Beelzebumon.bin", PreTransformMatrix))))
		return E_FAIL;
#pragma endregion

	//------------------------------------------------------------------------------------------------------------------------------

	///* For.Prototype_GameObject_Tennis */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tennis"),
	//	CTennis::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldA*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldA"),
	//	CBldA::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldB*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldB"),
	//	CBldB::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldC*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldC"),
	//	CBldC::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldD*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldD"),
	//	CBldD::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldE*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldE"),
	//	CBldE::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldH*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldH"),
	//	CBldH::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldJ*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldJ"),
	//	CBldJ::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BldK*/
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BldK"),
	//	CBldK::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Spawner*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Spawner"),
		CSpawner::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tribune*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tribune"),
		CTribune::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TennisLight*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TennisLight"),
		CTennisLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//디지몬 스킬---------------------------------------------------------------------------------------------------------------------------


	/* For.Prototype_Component_Model_fild*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_fild"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/All/fild.Bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Thumder */
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Thumder"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/All/Thumder.Bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Disc */
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Disc"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/All/Disc.Bin", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Plane*/
	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(90.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Plane"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/All/Plane.Bin", PreTransformMatrix))))
		return E_FAIL;


	/* For.Prototype_GameObject_Skill_Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skill_Model"),
		CSkill_Model::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Skill_Model*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skill_Effect_Image"),
		CSkill_Effect_Image::Create(m_pDevice, m_pContext))))
		return E_FAIL;







#pragma endregion
	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

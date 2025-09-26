#include "Digimon_Manager.h"
#include "PartyUHD.h"
#include "Digivice.h"
#include "ContainerObject.h"
#include "GameInstance.h"
#include "Player.h"


IMPLEMENT_SINGLETON(CDigimon_Manager)

CDigimon_Manager::CDigimon_Manager()
{
}

HRESULT CDigimon_Manager::Initialize()
{
	m_iDigivice_Number = 8;

	m_vDigivice_Battle_Slot.resize(m_iDigivice_Number);
	Not_found = L"";

	m_pGameInstance = CGameInstance::GetInstance();

	Model_Name();
	Digimon_Prototypr();
	Digimon_Info_Add();
	return S_OK;
}

void CDigimon_Manager::Digimon_Info_Add()
{
	Angewomon();
	Blackwargreymon();
	Devilmon();
	LedyDevimon();
	Leomon();
	Metalgarumon();
	Metalgeymon();
	Omegamoon();
	Wargreymon();
	Beelzebumon();
}

void CDigimon_Manager::Angewomon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("엔젤우몬");
	Info.DigimonId = 0;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("아름다운 여성의 모습을 한 대천사형 디지몬");
	Info.Hp = 5000;
	Info.Sp = 100;
	Info.Damage = 500;
	Info.AttackSpeed = 100;
	Info.Exp = 0;
	Info.Lv = 50;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Blackwargreymon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("블렉워그레이몬");
	Info.DigimonId = 1;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("칠흑의 용 전사 라고 불리는 두려운 존재인 바이러스종 워그레이몬");
	Info.Hp = 7000;
	Info.Sp = 130;
	Info.Damage = 1000;
	Info.AttackSpeed = 130;
	Info.Exp = 0;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Devilmon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("데빌몬");
	Info.DigimonId = 2;
	Info.Stage = DIGIMON_STAGE::CHAMPION;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("칠흑의 옷으로 몸을 감싸고 있는 타천사형 디지몬");
	Info.Hp = 2500;
	Info.Sp = 100;
	Info.Damage = 300;
	Info.AttackSpeed = 70;
	Info.Exp = 0;
	Info.Lv = 15;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::LedyDevimon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("레이디데블몬");
	Info.DigimonId = 3;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("고귀한 존재인 여성형 타천사 디지몬");
	Info.Hp = 5000;
	Info.Sp = 100;
	Info.Damage = 500;
	Info.AttackSpeed = 100;
	Info.Exp = 0;
	Info.Lv = 50;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Leomon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("레오몬");
	Info.DigimonId = 4;
	Info.Stage = DIGIMON_STAGE::CHAMPION;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("백수의 왕, 고상한 용사라 불리는 수인형 디지몬");
	Info.Hp = 2500;
	Info.Sp = 100;
	Info.Damage = 300;
	Info.AttackSpeed = 70;
	Info.Exp = 0;
	Info.Lv = 15;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Metalgarumon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("메탈가루몬");
	Info.DigimonId = 5;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::DATA;
	Info.DigimonInfo = TEXT("전신의 대부분을 메탈화 한 것으로 파워업한 가루몬의 최종형태");
	Info.Hp = 7000;
	Info.Sp = 130;
	Info.Damage = 1000;
	Info.AttackSpeed = 130;
	Info.Exp = 0;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Metalgeymon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("메탈그레이몬");
	Info.DigimonId = 6;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("몸의 절반 이상을 기계화한 사이보그형 디지몬");
	Info.Hp = 5000;
	Info.Sp = 100;
	Info.Damage = 500;
	Info.AttackSpeed = 100;
	Info.Exp = 0;
	Info.Lv = 50;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Omegamoon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("오메가몬");
	Info.DigimonId = 7;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("선을 바라는 사람들의 강한 의지에 의해서 융햡해 탄생한 성기사형 디지몬");
	Info.Hp = 10000;
	Info.Sp = 150;
	Info.Damage = 1300;
	Info.AttackSpeed = 150;
	Info.Exp = 0;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Wargreymon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("워그레이몬");
	Info.DigimonId = 8;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("초금속 크롬디지조이드의 값옷을 입고 있는 최강의 용전사");
	Info.Hp = 7000;
	Info.Sp = 130;
	Info.Damage = 1000;
	Info.AttackSpeed = 130;
	Info.Exp = 0;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Beelzebumon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("베르제브몬");
	Info.DigimonId = 9;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("많은 악마형 디지몬을 통괄하는 능력을 가지면서도 고고하게 존재하는 마왕형 디지몬");
	Info.Hp = 7000;
	Info.Sp = 130;
	Info.Damage = 1000;
	Info.AttackSpeed = 130;
	Info.Exp = 0;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Swap_Digimon(_int Sour, _int Dest)
{
	if (m_vDigivice_Battle_Slot[Dest] == false)
	{

	}
}

void CDigimon_Manager::Digimon_UHD()
{

}

void CDigimon_Manager::Acquire_Digimon(_int ID)
{
	m_pDigivice->Acquire_Digimon(ID);
}

void CDigimon_Manager::Release_Digimon(_int ID)
{
	m_pDigivice->Release_Digimon(ID);
}

void CDigimon_Manager::Set_Digivice_Slot(_int SlotID, _bool Has, _int Digimon_ID)
{
	m_vDigivice_Battle_Slot[SlotID] = Has;
	if (SlotID <= 0)
	{
		m_pFirst_Digimon = static_cast<CContainerObject*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), Get_Prototype_ID(Digimon_ID), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Digimon")));
		static_cast<CPlayer*>(m_pPlayer)->First_Digimon(m_pFirst_Digimon);
	}

	if (SlotID <= 2)
	{
		m_pPartyUHD->Set_Digimon_ID(SlotID, Has,Digimon_ID);
	}
}

DIGIMON_INFO* CDigimon_Manager::Search_Digimon(_int ID)
{
	auto iter = Find_Digimon(ID);
		
	if (nullptr == iter)
		return nullptr;

	return iter;
}

const _wstring& CDigimon_Manager::Get_Model_ID(_int ID)
{
	auto iter = Find_Model(ID);

	if (nullptr == iter)
		return Not_found;

	return *iter;
}

const _wstring& CDigimon_Manager::Get_Prototype_ID(_int ID)
{
	auto iter = Find_Prototype(ID);

	if (nullptr == iter)
		return Not_found;

	return *iter;
}

void CDigimon_Manager::PartyUHD(CPartyUHD* pPartyUHD)
{
	m_pPartyUHD = pPartyUHD;
}

void CDigimon_Manager::Digivice(CDigivice* pDigivice)
{
	m_pDigivice = pDigivice;
}

void CDigimon_Manager::Player(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}

_vector CDigimon_Manager::PlayerPos()
{
	return m_pPlayer->Get_Transform();
}

HRESULT CDigimon_Manager::Digimon_Add(_int Digimon_ID, DIGIMON_INFO Info)
{
	if (nullptr != Find_Digimon(Digimon_ID))
		return E_FAIL;

	m_Digimon_Info.emplace(Digimon_ID, Info);

	return S_OK;
}

void CDigimon_Manager::Model_Name()
{
	m_Digimon_Model.emplace(0,TEXT("Prototype_Component_Model_Angewomon"));
	m_Digimon_Model.emplace(1,TEXT("Prototype_Component_Model_Blackwargreymon"));
	m_Digimon_Model.emplace(2,TEXT("Prototype_Component_Model_Devilmon"));
	m_Digimon_Model.emplace(3,TEXT("Prototype_Component_Model_Ladydevimon"));
	m_Digimon_Model.emplace(4,TEXT("Prototype_Component_Model_Leomon"));
	m_Digimon_Model.emplace(5,TEXT("Prototype_Component_Model_Metalgarumon"));
	m_Digimon_Model.emplace(6,TEXT("Prototype_Component_Model_Metalgreymon"));
	m_Digimon_Model.emplace(7,TEXT("Prototype_Component_Model_Omegamon"));
	m_Digimon_Model.emplace(8,TEXT("Prototype_Component_Model_Wargreymon"));
	m_Digimon_Model.emplace(9,TEXT("Prototype_Component_Model_Beelzebumon"));
}

void CDigimon_Manager::Digimon_Prototypr()
{
	m_Digimon_Prototype.emplace(0, TEXT("Prototype_GameObject_Angewomon"));
	m_Digimon_Prototype.emplace(1, TEXT("Prototype_GameObject_Blackwargreymon"));
	m_Digimon_Prototype.emplace(2, TEXT("Prototype_GameObject_Devilmon"));
	m_Digimon_Prototype.emplace(3, TEXT("Prototype_GameObject_Ladydevimon"));
	m_Digimon_Prototype.emplace(4, TEXT("Prototype_GameObject_Leomon"));
	m_Digimon_Prototype.emplace(5, TEXT("Prototype_GameObject_Metalgarumon"));
	m_Digimon_Prototype.emplace(6, TEXT("Prototype_GameObject_Metalgreymon"));
	m_Digimon_Prototype.emplace(7, TEXT("Prototype_GameObject_Omegamon"));
	m_Digimon_Prototype.emplace(8, TEXT("Prototype_GameObject_Wargreymon"));
	m_Digimon_Prototype.emplace(9, TEXT("Prototype_GameObject_Beelzebumon"));
}

DIGIMON_INFO* CDigimon_Manager::Find_Digimon(_int Digimon_ID)
{
	auto iter = m_Digimon_Info.find(Digimon_ID);

	if (iter == m_Digimon_Info.end())
		return nullptr;

	return &iter->second;
}

const _wstring* CDigimon_Manager::Find_Model(_int Digimon_ID)
{
	auto iter = m_Digimon_Model.find(Digimon_ID);

	if (iter == m_Digimon_Model.end())
		return nullptr;

	return &iter->second;
}

const _wstring* CDigimon_Manager::Find_Prototype(_int Digimon_ID)
{
	auto iter = m_Digimon_Prototype.find(Digimon_ID);

	if (iter == m_Digimon_Prototype.end())
		return nullptr;

	return &iter->second;
}

void CDigimon_Manager::Free()
{
	__super::Free();

	//for (auto& Pair : m_Digimon_Info)
	//	Safe_Release(Pair.second);

	Safe_Release(m_pPlayer);

	m_vDigivice_Battle_Slot.clear();
}

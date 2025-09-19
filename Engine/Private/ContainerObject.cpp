#include "ContainerObject.h"
#include "PartObject.h"

#include "GameInstance.h"

CContainerObject::CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CContainerObject::CContainerObject(const CContainerObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CContainerObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CContainerObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CContainerObject::Priority_Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Priority_Update(fTimeDelta);
}

void CContainerObject::Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Update(fTimeDelta);
}

void CContainerObject::Late_Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Late_Update(fTimeDelta);
}

HRESULT CContainerObject::Render()
{


	return S_OK;
}

wstring CContainerObject::Get_Digimon_Name()
{
	return Info.DigimonName;
}

wstring CContainerObject::Get_Digimon_Map_Name()
{
	return Info.Digimon_Map;
}

_int CContainerObject::Get_ID()
{
	return Info.DigimonId;
}

_int CContainerObject::Get_Hp()
{
	return Info.Hp;
}

_int CContainerObject::Get_Sp()
{
	return Info.Sp;
}

_int CContainerObject::Get_Damage()
{
	return Info.Damage;
}

_int CContainerObject::Get_AttackSpeed()
{
	return Info.AttackSpeed;
}

_int CContainerObject::Get_Exp()
{
	return Info.Exp;
}

_int CContainerObject::Get_Lv()
{
	return Info.Lv;
}

DIGIMON_STAGE CContainerObject::Get_Stage()
{
	return Info.Stage;
}

DIGIMON_ATTRIBUTE CContainerObject::Get_Attribute()
{
	return Info.Attribute;
}

wstring CContainerObject::Get_Digimon_Info()
{
	return Info.DigimonInfo;
}

void CContainerObject::Set_Hp(_int Hp)
{

}

void CContainerObject::Set_Sp(_int Sp)
{

}

void CContainerObject::Set_Damage(_int Damage)
{

}

void CContainerObject::Set_AttackSpeed(_int AttackSpeed)
{

}

void CContainerObject::Set_Exp(_int Exp)
{

}

void CContainerObject::Set_Lv(_int Lv)
{

}

CPartObject* CContainerObject::Find_PartObject(const _wstring& strPartTag)
{
	auto    iter = m_PartObjects.find(strPartTag);

	if (iter == m_PartObjects.end())
		return nullptr;

	return iter->second;
}

HRESULT CContainerObject::Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg)
{
	if (nullptr != Find_PartObject(strPartTag))
		return E_FAIL;

	CPartObject* pPartObject = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));

	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects.emplace(strPartTag, pPartObject);

	return S_OK;
}

void CContainerObject::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);

	m_PartObjects.clear();
}


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

CComponent* CContainerObject::Get_Component(const _wstring& strPartTag, const _wstring& strComponentTag)
{
	CPartObject* pPartObject = Find_PartObject(strPartTag);
	if (nullptr == pPartObject)
		return nullptr;

	return pPartObject->Find_Component(strComponentTag);
}

wstring CContainerObject::Get_Digimon_Name()
{
	return Info.DigimonName;
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

void CContainerObject::Set_Digimon_Info(DIGIMON_INFO Digimon_Info)
{
	m_strDigimon_Name = Digimon_Info.DigimonName;
	m_iDigimon_ID = Digimon_Info.DigimonId;
	m_eState = Digimon_Info.Stage;
	m_eAttribute = Digimon_Info.Attribute;
	m_strDigimon_Info = Digimon_Info.DigimonInfo;
	m_iHp = Digimon_Info.Hp;
	m_iSp = Digimon_Info.Sp;
	m_iDamage = Digimon_Info.Damage;
	m_iAttackSpeed = Digimon_Info.AttackSpeed;
	m_iExp = Digimon_Info.Exp;
	m_iLv = Digimon_Info.Lv;
}

void CContainerObject::Skill1()
{
}

void CContainerObject::Skill2()
{
}

void CContainerObject::Skill3()
{
}

void CContainerObject::LookAt(_float iRadian)
{
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(iRadian));
}

void CContainerObject::Set_Position(_float fX, _float fZ)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(fX, 0.f, fZ, 1.f));
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


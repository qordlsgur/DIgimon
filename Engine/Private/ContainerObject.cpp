#include "ContainerObject.h"
#include "PartObject.h"
#include "Collider.h"
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

	m_bBattle = false;
	m_bMonster = false;
	m_bSkill = false;
	m_bSkillMove = false;
	m_bBackJump = false;
	m_bSkill1 = false;
	m_bSkill2 = false;
	m_bSkill3 = false;
	m_bTurnEnd = false;
	m_bLife = true;

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

const wstring& CContainerObject::Get_Digimon_Name()
{
	return m_strDigimon_Name;
}

_int CContainerObject::Get_ID()
{
	return m_iDigimon_ID;
}

_int CContainerObject::Get_Hp()
{
	return m_iHp;
}

_int CContainerObject::Get_Sp()
{
	return m_iSp;
}

_int CContainerObject::Get_Damage()
{
	return m_iDamage;
}

_int CContainerObject::Get_AttackSpeed()
{
	return m_iAttackSpeed;
}

_int CContainerObject::Get_Exp()
{
	return m_iExp;
}

_int CContainerObject::Get_Lv()
{
	return m_iLv;
}

DIGIMON_INFO CContainerObject::CurrentInfo()
{
	return Info;
}

DIGIMON_STAGE CContainerObject::Get_Stage()
{
	return m_eState;
}

DIGIMON_ATTRIBUTE CContainerObject::Get_Attribute()
{
	return m_eAttribute;
}

const wstring& CContainerObject::Get_Digimon_Info()
{
	return m_strDigimon_Info;
}

void CContainerObject::Set_Digimon_Info(DIGIMON_INFO* Digimon_Info)
{
	m_strDigimon_Name = Digimon_Info->DigimonName;
	m_iDigimon_ID = Digimon_Info->DigimonId;
	m_eState = Digimon_Info->Stage;
	m_eAttribute = Digimon_Info->Attribute;
	m_strDigimon_Info = Digimon_Info->DigimonInfo;
	m_iHp = Digimon_Info->Hp;
	m_iSp = Digimon_Info->Sp;
	m_iDamage = Digimon_Info->Damage;
	m_iAttackSpeed = Digimon_Info->AttackSpeed;
	m_iExp = Digimon_Info->Exp;
	m_iLv = Digimon_Info->Lv;
}

void CContainerObject::Set_Hp(_int Hp)
{
	m_iHp += Hp;
}

void CContainerObject::Set_Sp(_int Sp)
{
	m_iSp += Sp;
}

void CContainerObject::Set_Damage(_int Damage)
{
	m_iDamage += Damage;
}

void CContainerObject::Set_AttackSpeed(_int AttackSpeed)
{
	m_iAttackSpeed += static_cast<int>(m_eState) * AttackSpeed;
}

void CContainerObject::Set_Exp(_int Exp)
{
	m_iExp = (1000 * static_cast<int>(m_eState)) + Exp;
}

void CContainerObject::Set_Lv(_int Lv)
{
	int tens = Lv / 10;
	int ones = Lv % 10;

	m_iLv += static_cast<int>(m_eState) * tens + ones;
}

void CContainerObject::UseSkill(_int Skill)
{
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


_vector CContainerObject::Get_Position()
{
	return m_pTransformCom->Get_State(STATE::POSITION);
}

void CContainerObject::LookAt(_float iRadian)
{
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(iRadian));
}

void CContainerObject::LookAt(_vector Pos)
{
	m_pTransformCom->LookAt(Pos);
}

void CContainerObject::Set_Position(_float fX, _float fZ)
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(fX, 0.f, fZ, 1.f));
}

void CContainerObject::Set_y(_float fY)
{
	m_pTransformCom->Set_Y(fY);
}

_int CContainerObject::Intersect(CCollider* pPlayer_Collider)
{
	return _int();
}

void CContainerObject::Attack_Move(_vector Pos, _float fTimeDelta)
{
	//m_pTransformCom->
}

void CContainerObject::Target_Pos_Move(_fvector Target_Pos, _float fTimeDelta)
{
	m_pTransformCom->Target_Pos_Move(Target_Pos, fTimeDelta);
}

_bool CContainerObject::HasReachedTargetPosition(_vector Pos)
{
	_vector currentPos = m_pTransformCom->Get_State(STATE::POSITION);

	// 두 벡터 간 거리 계산
	_vector diff = Pos - currentPos;
	float distance = XMVectorGetX(XMVector3Length(diff)); // 거리

	float threshold = 0.2f;

	// threshold 이내면 도착
	if (distance <= threshold)
		return false;

	return true;
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


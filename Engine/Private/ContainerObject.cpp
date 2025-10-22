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
	if (Info.CurrentHp <= 0)
	{
		m_bDie = true;
		Info.CurrentHp = 0;
	}
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

_int CContainerObject::Get_CurrentHp()
{
	return Info.CurrentHp;
}

_int CContainerObject::Get_Sp()
{
	return Info.Sp;
}

_int CContainerObject::Get_CurrentSp()
{
	return Info.CurrentSp;
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
	return Info.ExpReward;
}

_int CContainerObject::Get_Lv()
{
	return Info.Lv;
}

DIGIMON_INFO& CContainerObject::CurrentInfo()
{
	return Info;
}

DIGIMON_STAGE CContainerObject::Get_Stage()
{
	return Info.Stage;
}

DIGIMON_ATTRIBUTE CContainerObject::Get_Attribute()
{
	return Info.Attribute;
}

const wstring& CContainerObject::Get_Digimon_Info()
{
	return Info.DigimonInfo;
}

void CContainerObject::Set_Digimon_Info(DIGIMON_INFO* Digimon_Info)
{
	Info = *Digimon_Info;
}

void CContainerObject::Set_Hp(_int Hp)
{
	Info.Hp += Hp;
	Info.CurrentHp += Hp;
}

void CContainerObject::Set_Sp(_int Sp)
{
	Info.Sp += Sp;
	Info.CurrentSp += Sp;
}

void CContainerObject::Set_Damage(_int Damage)
{
	Info.Damage += Damage;
}

void CContainerObject::Set_AttackSpeed(_int AttackSpeed)
{
	Info.AttackSpeed += static_cast<int>(Info.Stage) * AttackSpeed;
}

void CContainerObject::Set_Exp(_int Exp)
{
	Info.ExpReward = (1000 * static_cast<int>(Info.Stage)) + Exp;
}

void CContainerObject::Set_Lv(_int Lv)
{
	int tens = Lv / 10;
	int ones = Lv % 10;

	Info.Lv += static_cast<int>(Info.Stage) * tens + ones;
}

void CContainerObject::Gain_Exp(_int Exp)
{
	Info.CurrentExp += Exp;
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

_int CContainerObject::Skill_Damage()
{
	return _int();
}

void CContainerObject::Set_HitDamage(_int Damage)
{
	m_iHitDamage = Damage;
	Info.CurrentHp -= m_iHitDamage;
	if (Info.CurrentHp < m_iHitDamage)
		Info.CurrentHp = 0;
	m_bHit = true;
}

void CContainerObject::Set_Dissolve(_bool Dissolve)
{
}

void CContainerObject::Creat_Skill(_int SkillNum)
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


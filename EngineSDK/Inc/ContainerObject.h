#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CContainerObject abstract : public CGameObject
{
protected:
	CContainerObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CContainerObject(const CContainerObject& Prototype);
	virtual ~CContainerObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	CComponent* Get_Component(const _wstring& strPartTag, const _wstring& strComponentTag);

public:

	virtual const wstring& Get_Digimon_Name();
	virtual _int Get_ID();
	virtual DIGIMON_STAGE Get_Stage();
	virtual DIGIMON_ATTRIBUTE Get_Attribute();
	virtual const wstring& Get_Digimon_Info();
	virtual _int Get_Hp();
	virtual _int Get_Sp();
	virtual _int Get_Damage();
	virtual _int Get_AttackSpeed();
	virtual _int Get_Exp();
	virtual _int Get_Lv();

	virtual DIGIMON_INFO& CurrentInfo();

	virtual void Set_Digimon_Info(DIGIMON_INFO* Digimon_Info);

	virtual void Set_Hp(_int Hp);
	virtual void Set_Sp(_int Sp);
	virtual void Set_Damage(_int Damage);
	virtual void Set_AttackSpeed(_int AttackSpeed);
	virtual void Set_Exp(_int Exp);
	virtual void Set_Lv(_int Lv);
	virtual void Gain_Exp(_int Exp);

	virtual void UseSkill(_int Skill);
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();

	virtual void Set_Battle(_bool Battle) { m_bBattle = Battle; }
	virtual _bool Get_Battle() { return m_bBattle; }

	virtual void Set_Monster(_bool Monster) { m_bMonster = Monster; }
	virtual _bool Get_Monster() { return m_bMonster; }

	virtual void Set_Life(_bool Life) { m_bLife = Life; }
	virtual _bool Get_Life() { return m_bLife; }

	virtual _bool Get_Skill1() { return m_bSkill1; }
	virtual _bool Get_Skill2() { return m_bSkill2; }
	virtual _bool Get_Skill3() { return m_bSkill3; }
	virtual void Set_SkillMove(_bool Move) { m_bSkillMove = Move; }
	virtual _bool Get_SkillMove() { return m_bSkillMove; }

	virtual void Set_BackJump(_bool BackJump) { m_bBackJump = BackJump; }
	virtual _bool Get_BackJump() { return m_bBackJump; }

	virtual void Set_TurnEnd(_bool End) { m_bTurnEnd = End; }
	virtual _bool Get_TurnEnd() { return m_bTurnEnd; }

	virtual _vector Get_Position();

	virtual void LookAt(_float iRadian);
	virtual void LookAt(_vector Pos);
	virtual void Set_Position(_float fX, _float fZ);
	virtual void Set_y(_float fY);

	virtual _int Intersect(class CCollider* pPlayer_Collider);

	virtual void Attack_Move(_vector Pos, _float fTimeDelta);

	virtual void Target_Pos_Move(_fvector Target_Pos, _float fTimeDelta);

	virtual _bool HasReachedTargetPosition(_vector Pos);

	virtual _int Skill_Damage();
	virtual void Set_HitDamage(_int Damage);

protected:
	map<const _wstring, class CPartObject*>			m_PartObjects;

	DIGIMON_INFO Info;

	_int m_iDamage{};
	_int m_iHitDamage{};

	_bool m_bBattle = { false };
	_bool m_bMonster = { false };
	_bool m_bLife = { true };
	_bool m_bSkill = { false };
	_bool m_bSkillMove = { false };
	_bool m_bBackJump = { false };

	_bool m_bSkill1 = { false };
	_bool m_bSkill2 = { false };
	_bool m_bSkill3 = { false };
	_bool m_bTurnEnd = { true };

	queue<DIGIMONSTATE> m_eSkill_State;

protected:
	class CPartObject* Find_PartObject(const _wstring& strPartTag);
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg = nullptr);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END
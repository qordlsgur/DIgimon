#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CContainerObject abstract : public CGameObject
{
protected:
	CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
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

	virtual DIGIMON_INFO CurrentInfo();

	virtual void Set_Digimon_Info(DIGIMON_INFO* Digimon_Info);

	virtual void Set_Hp(_int Hp);
	virtual void Set_Sp(_int Sp);
	virtual void Set_Damage(_int Damage);
	virtual void Set_AttackSpeed(_int AttackSpeed);
	virtual void Set_Exp(_int Exp);
	virtual void Set_Lv(_int Lv);

	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();

	virtual void Set_Battle(_bool Battle) { m_bBattle = Battle; }
	virtual _bool Get_Battle() { return m_bBattle; }

	virtual void Set_Monster(_bool Monster) { m_bMonster= Monster; }
	virtual _bool Get_Monster() { return m_bMonster; }

	virtual void Set_Life(_bool Life) { m_bLife = Life; }
	virtual _bool Get_Lide() { return m_bLife; }

	virtual void LookAt(_float iRadian);
	virtual void Set_Position(_float fX, _float fZ);

	virtual _int Intersect(class CCollider* pPlayer_Collider);

protected:
	map<const _wstring, class CPartObject*>			m_PartObjects;

	DIGIMON_INFO Info;

	wstring m_strDigimon_Name;
	_int m_iDigimon_ID{};
	DIGIMON_STAGE m_eState = DIGIMON_STAGE::END;
	DIGIMON_ATTRIBUTE m_eAttribute = DIGIMON_ATTRIBUTE::END;
	wstring m_strDigimon_Info;
	_int m_iHp{};
	_int m_iSp{};
	_int m_iDamage{};
	_int m_iAttackSpeed{};
	_int m_iExp{};
	_int m_iLv{};

	_bool m_bBattle = { false };
	_bool m_bMonster = { false };
	_bool m_bLife = { true };

protected:
	class CPartObject* Find_PartObject(const _wstring& strPartTag);
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg = nullptr);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END
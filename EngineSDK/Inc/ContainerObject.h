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

	virtual wstring Get_Digimon_Name();
	virtual wstring Get_Digimon_Map_Name();
	virtual _int Get_ID();
	virtual DIGIMON_STAGE Get_Stage();
	virtual wstring Get_Digimon_Info();
	virtual DIGIMON_ATTRIBUTE Get_Attribute();
	virtual _int Get_Hp();
	virtual _int Get_Sp();
	virtual _int Get_Damage();
	virtual _int Get_AttackSpeed();
	virtual _int Get_Exp();
	virtual _int Get_Lv();


	virtual void Set_Hp(_int Hp);
	virtual void Set_Sp(_int Sp);
	virtual void Set_Damage(_int Damage);
	virtual void Set_AttackSpeed(_int AttackSpeed);
	virtual void Set_Exp(_int Exp);
	virtual void Set_Lv(_int Lv);

protected:
	map<const _wstring, class CPartObject*>			m_PartObjects;

	DIGIMON_INFO Info;

protected:
	class CPartObject* Find_PartObject(const _wstring& strPartTag);
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg = nullptr);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END
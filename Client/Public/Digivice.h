#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice final : public CUIObject
{
private:
	CDigivice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice(const CDigivice& Prototype);
	virtual ~CDigivice() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	//void Digimon_Swap();

	void Acquire_Digimon(_int ID);
	void Release_Digimon(_int ID);

	virtual void OnClick() override;
	virtual void OnHover() override;

	void Update_Digimopn();

	DIGIMON_INFO Set_Info(_int ID);

private:
	void Set_State(DIGIMON_INFO ID);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };	
	CShader* m_pShaderCom = { nullptr };

	class CDigivice_Slot* m_pDigivice_Slot = { nullptr };
	class CDigivice_Mask* m_pDigivice_Mask = { nullptr };
	class CDigivice_Info* m_pDigivice_Info = { nullptr };
	class CDigivice_Skill* m_pDigivice_Skill = { nullptr };
	class CDigivice_Target* m_pDigivice_Target = { nullptr };

	class CDigivice_Hp*		m_pDigivice_Hp = { nullptr };
	class CDigivice_Sp*		m_pDigivice_Sp = { nullptr };
	class CDigivice_Exp*	m_pDigivice_Exp = { nullptr };

	_uint	m_iDigivice_Battle_Slot_Number{};
	_uint	m_iDigivice_Battle_Skill_Number{};

	_int m_iSlot_Number{};
	vector<class CDigivice_Slot*> m_pBattle_Slot;
	vector<class CDigivice_Mask*> m_pBattle_Mask;

	vector<class CDigivice_Skill*> m_pBattle_Skill;
	map<int, vector<class CDigivice_Skill*>> m_pSkills;
	_int m_iSkillCount{};

	vector<DIGIMON_INFO> m_pDigimon_Info;
	vector<_int> m_Digimon_ID;

	wstring	m_strDigimon_Stage[5] = {};
	wstring	m_strDigimon_Attribute[3] = {};

	_tchar			m_szDigimonHp[MAX_PATH] = {};
	_tchar			m_szDigimonSp[MAX_PATH] = {};
	_tchar			m_szDigimonMaxExp[MAX_PATH] = {};
	_tchar			m_szDigimonCurrentExp[MAX_PATH] = {};
	_tchar			m_szDigimonDamage[MAX_PATH] = {};
	_tchar			m_szDigimonAttackSpeed[MAX_PATH] = {};
	_tchar			m_szCurrentHp[MAX_PATH] = {};
	_tchar			m_szCurrentSp[MAX_PATH] = {};
	_tchar			m_szCurrentEXp[MAX_PATH] = {};

	DIGIMON_INFO* Info;

	class CDigimon_Manager* m_pDigimon_Manager = {nullptr};
	class CBattle_Manager* m_pBattle_Manager = {nullptr};
	class CDigivice_Slot* m_pSelectSlot = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Create_Slot(const _wstring& strLayerTag);
	HRESULT Create_Skill(const _wstring& strLayerTag, _int SlotID, _int DigimonID);
	HRESULT Create_Target(const _wstring& strLayerTag);
	HRESULT Craete_State(const _wstring& strLayerTag);

public:
	static CDigivice* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice_Skill final : public CUIObject
{
private:
	CDigivice_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice_Skill(const CDigivice_Skill& Prototype);
	virtual ~CDigivice_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Parent_WorldPos(_vector fParent_World);
	void Set_Move(_float fX, _float fY);

	void Set_HasDigimon(_bool Digimon) { m_bHasDigimon = Digimon; }
	_bool Get_HasDigimon() { return m_bHasDigimon; }

	void Set_Info(DIGIMON_INFO* pInfo);

	virtual void OnClick() override;

	//DIGIMON_INFO Get_DigimonInfo() { return m_Info; }

	HRESULT Set_Digimon_SkillSet(_int ID, _int Digimon_Skill);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pSlotTextureCom = { nullptr };
	CTexture* m_pDigimonSKillTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	class CDigimon_Manager* m_pManager = { nullptr };

	DIGIMON_INFO	m_Info;

	_bool			m_bHasDigimon = false;

	_int			m_iDigimon_Skill{};

	_wstring		m_strDigimon_SkillName;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	_float4 m_fParent_WorldPos{};

public:
	static CDigivice_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
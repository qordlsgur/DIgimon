#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice_Skill_Info final : public CUIObject
{
private:
	CDigivice_Skill_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice_Skill_Info(const CDigivice_Skill_Info& Prototype);
	virtual ~CDigivice_Skill_Info() = default;

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
	void Set_Digimon_Skill_Info(_int ID, _int Count);

	void Set_Info(SKILL_INFO* pInfo);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	class CDigimon_Manager* m_pManager = { nullptr };

	SKILL_INFO	m_Info;

	wstring	m_strDigimon_Terget[3] = {};
	wstring	m_strDigimon_Range[2] = {};

	_tchar			m_szSkillSp[MAX_PATH] = {};
	_tchar			m_szSkillCount[MAX_PATH] = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	_float4 m_fParent_WorldPos{};

public:
	static CDigivice_Skill_Info* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
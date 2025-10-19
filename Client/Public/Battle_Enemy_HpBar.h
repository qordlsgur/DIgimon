#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CBattle_Enemy_HpBar final : public CUIObject
{
private:
	CBattle_Enemy_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle_Enemy_HpBar(const CBattle_Enemy_HpBar& Prototype);
	virtual ~CBattle_Enemy_HpBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Parent_WorldPos(_vector fParent_World);

	void Set_MaxHp(_float MaxHp);
	void Set_Damage(_int Damage);
	void Set_Move(_float fX, _float fY);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pBgTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_tchar			m_szDigimonMaxHp[MAX_PATH] = {};
	_tchar			m_szCurrentHp[MAX_PATH] = {};

	_float4 Pos;
	_float m_iMaxHp{};
	_float m_iCurrentHp{};
	_float m_fHpRatio{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	_float4 m_fParent_WorldPos{};

public:
	static CBattle_Enemy_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
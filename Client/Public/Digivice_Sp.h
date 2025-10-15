#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice_Sp final : public CUIObject
{
private:
	CDigivice_Sp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice_Sp(const CDigivice_Sp& Prototype);
	virtual ~CDigivice_Sp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_MaxSp(_float MaxSp);
	void Set_Move(_float fX, _float fY);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pBgTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_float m_iMaxSp{};
	_float m_iCurrentSp{};
	_float m_fSpRatio{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDigivice_Sp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
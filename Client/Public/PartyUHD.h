#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "Slot.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CPartyUHD final : public CUIObject
{
private:
	CPartyUHD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartyUHD(const CPartyUHD& Prototype);
	virtual ~CPartyUHD() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_uint								m_iSlotCount{};

private:


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Create_Slot(const _wstring& strLayerTag);

public:
	static CPartyUHD* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


NS_END
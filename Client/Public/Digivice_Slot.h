#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice_Slot final : public CUIObject
{
private:
	CDigivice_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice_Slot(const CDigivice_Slot& Prototype);
	virtual ~CDigivice_Slot() = default;

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

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture*		m_pSlotTextureCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };

	_uint			m_iDigimon_ID{};


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	_float4 m_fParent_WorldPos{};

public:
	static CDigivice_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
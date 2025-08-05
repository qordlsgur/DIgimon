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

class CInventory final : public CUIObject
{
private:
	CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory(const CInventory& Prototype);
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect*						m_pVIBufferCom = { nullptr };
	CTexture*							m_pTextureCom = { nullptr };
	CShader*							m_pShaderCom = { nullptr };

	_uint								m_iSlotCount{};
	vector<class CSlot*>				m_vSlots;
	class CSlot*						m_pSlot = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Create_Slot(const _wstring& strLayerTag);

public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
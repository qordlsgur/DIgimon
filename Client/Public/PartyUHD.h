#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "Slot.h"

NS_BEGIN(Engine)

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

public:
	void Set_Digimon_ID(_uint Slot, _bool HasDigimon, _uint ID);

private:
	class CDigimon_Manager*					m_pDigimon_Manager = { nullptr };
	class CUHD_Slot*						m_pUHD_Slot		   = { nullptr };

	vector<class CUHD_Slot*>				m_vSlots;

	_uint									m_iSlotCount{};

private:

	HRESULT Create_Slot(const _wstring& strLayerTag);

public:
	static CPartyUHD* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


NS_END
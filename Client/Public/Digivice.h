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

	void OnClick();

	void Update_Digimopn();

	DIGIMON_INFO* Set_Info(_int ID);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	class CDigivice_Slot* m_pDigivice_Slot = { nullptr };
	class CDigivice_Mask* m_pDigivice_Mask = { nullptr };

	_uint	m_iDigivice_Battle_Slot_Number;

	vector<class CDigivice_Slot*> m_pBattle_Slot;
	vector<class CDigivice_Mask*> m_pBattle_Mask;
	vector<DIGIMON_INFO*> m_pDigimon_Info;
	vector<_int> m_Digimon_ID;


	class CDigimon_Manager* m_pDigimon_Manager = {nullptr};
	class CBattle_Manager* m_pBattle_Manager = {nullptr};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Create_Slot(const _wstring& strLayerTag);

public:
	static CDigivice* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
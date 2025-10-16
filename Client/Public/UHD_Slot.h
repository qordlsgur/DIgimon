#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CUHD_Slot final : public CUIObject
{
private:
	CUHD_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUHD_Slot(const CUHD_Slot& Prototype);
	virtual ~CUHD_Slot() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Move(_float fX, _float fY);
	_bool	Get_Digimon() { return m_bDigimon; }
	void	Set_Digimon(_bool HasDigimon) { m_bDigimon = HasDigimon; }
	virtual void OnClick() override;

	void Set_Digimon_Info(DIGIMON_INFO* Info);
	DIGIMON_INFO& Get_Info() { return m_Info; }

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture*  m_pFrameTextureCom = { nullptr };
	CTexture*  m_pMaskTextureCom = { nullptr };
	CTexture*  m_pSPTextureCom = { nullptr };
	CTexture*  m_pHPTextureCom = { nullptr };
	CTexture*  m_pEVPTextureCom = { nullptr };
	CTexture*  m_pDigimonTextureCom = { nullptr };
	CShader*   m_pShaderCom = { nullptr };
			   
	class CDigimon_Manager* m_pManager = { nullptr };

	DIGIMON_INFO m_Info;
			    
	_bool		m_bDigimon = false;



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CUHD_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
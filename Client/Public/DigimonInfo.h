//#pragma once
//
//#include "Client_Defines.h"
//#include "GameObject.h"
//
//NS_BEGIN(Engine)
//class CTexture;
//NS_END
//
//NS_BEGIN(Client)
//
//class CDigimonInfo abstract : public CGameObject
//{
//private:
//	CDigimonInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
//	CDigimonInfo(const CDigimonInfo& Prototype);
//	virtual ~CDigimonInfo() = default;
//
//public:
//	virtual HRESULT Initialize_Prototype() override;
//	virtual HRESULT Initialize(void* pArg) override;
//	virtual void Priority_Update(_float fTimeDelta) override;
//	virtual void Update(_float fTimeDelta) override;
//	virtual void Late_Update(_float fTimeDelta) override;
//	virtual HRESULT Render() override;
//
//private:
//	//CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
//	//CTexture* m_pTextureCom = { nullptr };
//	//CShader* m_pShaderCom = { nullptr };
//
//
//private:
//	HRESULT Ready_Components();
//	HRESULT Bind_ShaderResources();
//
//public:
//	static CDigimonInfo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
//	virtual CGameObject* Clone(void* pArg) override;
//	virtual void Free() override;
//
//};
//
//NS_END
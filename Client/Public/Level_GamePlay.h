#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Digivice(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UHD(const _wstring& strLayerTag);

	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Digimon(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Interaction(const _wstring& strLayerTag);



private:
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CCamera_Manager* m_pCamera_Manager = { nullptr };
	class CBattle_Manager* m_pBattle_Manager= { nullptr };
	class CInteraction_Manager* m_pIntertaction_Manager = { nullptr };
	class CBattle_UI_Manager* m_pBattle_UI_Manager = { nullptr };


public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID);
	virtual void Free() override;
};

NS_END
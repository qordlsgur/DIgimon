#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CDigiDex_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CDigiDex_Manager);

private:
	CDigiDex_Manager();
	virtual ~CDigiDex_Manager() = default;


public:
	HRESULT Initialize();

	void DigiDex(class CDigiDex* pDigiDex);

	//HRESULT Add_Digimon(const _wstring& strDigimon_Name, class CContainerObject)
private:

	class CDigiDex* m_pDigiDex = { nullptr };
	map<const wstring, class CContainerObject*> Digimons;

private:

private:
	virtual void Free() override;
};

NS_END
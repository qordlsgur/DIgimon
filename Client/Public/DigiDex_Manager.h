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
private:
	_bool Digimon_Data_Loder(const _tchar* pBinFilePath, vector<Digimon_Data>& Data);

	class CDigiDex* m_pDigiDex = { nullptr };

private:
	vector<Digimon_Data> Digimon_Datas;

private:
	virtual void Free() override;
};

NS_END
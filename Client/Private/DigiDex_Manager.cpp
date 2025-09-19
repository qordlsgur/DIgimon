#include "DigiDex_Manager.h"
#include "DigiDex.h"


IMPLEMENT_SINGLETON(CDigiDex_Manager)

CDigiDex_Manager::CDigiDex_Manager()
{
}

HRESULT CDigiDex_Manager::Initialize()
{


	return S_OK;
}

void CDigiDex_Manager::DigiDex(CDigiDex* pDigiDex)
{
	m_pDigiDex = pDigiDex;
}



void CDigiDex_Manager::Free()
{
	__super::Free();

	//Safe_Release(m_pDigiDex);
}
#include "DigiDex_Manager.h"


IMPLEMENT_SINGLETON(CDigiDex_Manager)

CDigiDex_Manager::CDigiDex_Manager()
{
}

HRESULT CDigiDex_Manager::Initialize()
{

	Digimon_Data_Loder(TEXT("../Bin/Digimon_Data/digimon.bin"), Digimon_Datas);

	return S_OK;
}

void CDigiDex_Manager::DigiDex(CDigiDex* pDigiDex)
{
	m_pDigiDex = pDigiDex;
}

_bool CDigiDex_Manager::Digimon_Data_Loder(const _tchar* pBinFilePath, vector<Digimon_Data>& Data)
{
	ifstream in(pBinFilePath, ios::binary);
	if (!in) {
		cerr << "바이너리 파일 열기 실패 (읽기)\n";
		return false;
	}

	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (!in) {
		cerr << "크기 읽기 실패\n";
		return false;
	}

	Data.resize(size);
	in.read(reinterpret_cast<char*>(Data.data()), sizeof(Digimon_Data) * size);
	if (!in) {
		cerr << "데이터 읽기 실패\n";
		return false;
	}

	return true;
}


void CDigiDex_Manager::Free()
{
	__super::Free();
}

#include "State.h"
#include "PartObject.h"

CState::CState()
{
}

void CState::Free()
{
	__super::Free();
	Safe_Release(m_pPart);
}

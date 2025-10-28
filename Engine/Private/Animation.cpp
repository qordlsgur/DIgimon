#include "Animation.h"
#include "Channel.h"
#include "Model.h"
#include "Bone.h"


CAnimation::CAnimation()
{
	/*XMMatrixDecompose();*/
}

CAnimation::CAnimation(const CAnimation& Prototype)
	: m_fCurrentTrackPosition{ Prototype.m_fCurrentTrackPosition }
	, m_fDuration{ Prototype.m_fDuration }
	, m_fTickPerSecond{ Prototype.m_fTickPerSecond }
	, m_iNumChannels{ Prototype.m_iNumChannels }
	, m_Channels{ Prototype.m_Channels }
	, m_CurrentKeyFrameIndices{ Prototype.m_CurrentKeyFrameIndices }
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);

}

HRESULT CAnimation::Initialize(CModel* pModel, ANIMATION& mAnim)
{
	_char MissName[MAX_PATH] = "StnadBattle";
	_char SkillName[MAX_PATH] = "StandBattle";
	// 애니메이션 이름
	strcpy_s(m_szCpyName, mAnim.mName);
	strcpy_s(m_szName, strchr(m_szCpyName, '|') + 1);
	if (strcmp(MissName, m_szName) == 0)
		strcpy_s(m_szName, SkillName);

	// 총 길이
	m_fDuration = mAnim.mfDuration;
	//한틱
	m_fTickPerSecond = mAnim.mfTicksPerSecond;
	// 뼈의 갯수
	m_iNumChannels = mAnim.miNumChannels;

	m_fBlendTime = 1.f;
	// 갯수 저장
	m_CurrentKeyFrameIndices.resize(m_iNumChannels);
	// 갯수를 알면 정보도 알 수 있음
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pModel, mAnim.mChannel[i]);
		if (nullptr == pChannel)
			return E_FAIL;
		// 정보를 저장
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrices(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta)
{
	/* 내 애니메이션의 현재 재생위치. */
	// 틱마다 중첩해서 값을 증가 시킴
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		if (true == isLoop)
			m_fCurrentTrackPosition = 0.f;
		else
		{
			m_bFnishi = true;
			return true;
		}
	}

	// 최대 길이보다 길면 0으로 초기화
	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		m_fCurrentTrackPosition = 0.f;
	}

	_uint		iIndex = {};

	for (auto& pChannel : m_Channels)
	{
		// 0이라 하면 0번에 본의 갯수만큼 돌려줌 한 프레임에 최대 갯수를 다 바꿔줌
		pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[iIndex++]);
	}

	return false;
}

void CAnimation::Change_TransformationMatrices(class CModel* pModel, const vector<class CBone*>& Bones, _float fTimeDelta, const vector<LERP>& mLerp)
{
	/* 내 애니메이션의 현재 재생위치. */
	// 틱마다 중첩해서 값을 증가 시킴
	m_fCurrentTrackPosition = 0.f;
	m_fSaveTime += fTimeDelta;
	m_fBlendTime = 0.2f;
	m_fTick = clamp(m_fSaveTime / m_fBlendTime, 0.f, 1.f);
	_uint		iIndex = {};

	for (auto& pChannel : m_NextKeyFrames)
	{
		pChannel->Change_TransformationMatrix(Bones, m_fTick, mLerp[iIndex++].mMatrix);
	}

	if (m_fTick >= 1.f)
	{
		pModel->Set_Anim();
		m_fSaveTime = 0.f;
	}

}

void CAnimation::Save_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, vector<LERP>& mLerp)
{
	_int		iIndex = {};
	mLerp.clear();
	for (auto& pChannel : m_Channels)
	{
		// 0이라 하면 0번에 본의 갯수만큼 돌려줌 한 프레임에 최대 갯수를 다 바꿔줌
		pChannel->Save_TransformationMatrix(Bones, m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[iIndex++], mLerp);
	}
}

void CAnimation::CompareStringVectors(vector<LERP>& mLerp)
{
	m_NextKeyFrames.clear();
	for (_uint i = 0; i < m_Channels.size(); ++i)
	{
		for (size_t j = 0; j < mLerp.size(); ++j)
		{
			if (strcmp(m_Channels[i]->Get_Name(), mLerp[j].mName) == 0)
			{
				m_NextKeyFrames.push_back(m_Channels[i]);
				break;
			}
		}
	}
}

void CAnimation::Set_Animamtion_Speed(_float iSpeed)
{
	m_fTickPerSecond = iSpeed;
}


void CAnimation::Reset()
{
	m_fCurrentTrackPosition = 0.f;

	for (auto& iKeyFrameIndex : m_CurrentKeyFrameIndices)
		iKeyFrameIndex = 0;
}

CAnimation* CAnimation::Create(CModel* pModel, ANIMATION& mAnim)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pModel, mAnim)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

	m_CurrentKeyFrameIndices.clear();

}

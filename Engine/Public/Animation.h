#pragma once

#include "Base.h"

/* 이 애니메이션을 구동하기위해 갱신해야할 뼈(Channel)의 갯수, 뼈의 시간대별 상태값, 애니메이션의 전체 길이, 재생 속도  */

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(class CModel* pModel, ANIMATION& mAnim);
	void Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta);

	void Change_TransformationMatrices(class CModel* pModel, const vector<class CBone*>& Bones, _float fTimeDelta, const vector<LERP>& mLerp);
	void Save_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, vector<LERP>& mLerp);
	void CompareStringVectors(vector<LERP>& mLerp);

	void Reset();
private:
	_char					m_szName[MAX_PATH] = {};

	_float						m_fCurrentTrackPosition = {};
	/* 애니메이션 재생을 위한 전체 길이 */
	_float						m_fDuration = {};
	/* 초당 얼마나 재생되어야하는지 : 재생 속도 */
	_float						m_fTickPerSecond = {};

	_float						m_fSaveTime{};
	_float						m_fBlendTime{};
	_float						m_fTick{};
	_float						m_Lerp{};

	_bool						m_bStart = true;

	_uint						m_iNumChannels = {};
	vector<class CChannel*>		m_Channels;

	vector<class CChannel*>		m_NextKeyFrames;
	vector<_uint>				m_CurrentKeyFrameIndices;

public:
	static CAnimation* Create(class CModel* pModel, ANIMATION& mAnim);
	CAnimation* Clone();
	virtual void Free() override;
};

NS_END


#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HINSTANCE			hInstance;
		HWND				hWnd;
		unsigned int		iNumLevels;
		unsigned int		iWinSizeX, iWinSizeY;
		WINMODE				eWindowMode;
	}ENGINE_DESC;

	typedef struct tagLightDesc
	{
		LIGHT			eType;
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		XMFLOAT4		vDirection;
		XMFLOAT4		vPosition;
		float			fRange;
	}LIGHT_DESC;

	typedef struct tagVertexPosition
	{
		XMFLOAT3			vPosition;

		static constexpr unsigned int					iNumElements = { 1 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

		};

	}VTXPOS;

	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	}VTXPOSTEX;

	typedef struct tagVertexCube
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vTexcoord;

		static constexpr unsigned int					iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	}VTXCUBE;

	typedef struct tagVertexPositionNormalTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXNORTEX;

	typedef struct tagVertexMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 4 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXMESH;


	typedef struct tagVertexAnimationMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexcoord;

		/* 이 정점이 어떤 뼈들의 상태를 받아야하는가? */
		XMUINT4				vBlendIndex;
		/* 각 뼈의 상태가 어떤 비율로 적용되야할지? */
		XMFLOAT4			vBlendWeight;

		static constexpr unsigned int					iNumElements = { 6 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTXANIMMESH;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vTranslation;
		float			fTrackPosition;
	}KEYFRAME;

	typedef struct LerpAnim
	{
		char mName[MAX_PATH];
		XMFLOAT4X4 mMatrix{};
	}LERP;

	typedef struct Material_Type_Data
	{
		unsigned int textureCount{};
		vector<wstring> szPerfectPath;
	}MATERIAL_TYPE;

	typedef struct Material_Data
	{
		unsigned int Type_Count{};
		vector<MATERIAL_TYPE> Path;
	}MATERIAL_DATA;

	typedef struct Vertex_Data
	{
		XMFLOAT3 mVertices{};
		XMFLOAT3 mNormals{};
		XMFLOAT3 mTangents{};
		XMFLOAT2 mTextureCoords{};
	}VERTEX;

	typedef struct Index_Data
	{
		unsigned int mIndices[3];
	}INDEX;

	typedef struct Mesh_Data
	{
		char mName[MAX_PATH];
		unsigned int mMaterialIndex{};
		unsigned int mNumVertices{};
		unsigned int mNumIndices{};
		unsigned int mNumFaces{};
		vector<VERTEX> mVertex;
		vector<INDEX> mIndex;
	}MESH;

	typedef struct NonAnim_Model_Data
	{
		unsigned int mNumMeshes{};
		unsigned int mNumMaterials{};
		vector<MESH> m_Mesh;
		vector<MATERIAL_DATA> Material_Data;
	}NONANIM_DATA;

	typedef struct Weight
	{
		unsigned int mVertexId{};
		float x{};
		float y{};
		float z{};
		float w{};
	}WEIGHT;

	typedef struct Bone
	{
		XMFLOAT4X4 mOffsetMatrix{};
		int mBoneIndex{};
		vector<WEIGHT> mWeight;

	}BONE;

	typedef struct AnimMesh_Data
	{
		char mName[MAX_PATH];
		unsigned int mMaterialIndex{};
		unsigned int mNumVertices{};
		unsigned int mNumIndices{};
		unsigned int mNumFaces{};
		unsigned int mNumBones{};
		vector<VERTEX> mVertex;
		vector<INDEX> mIndex;
		vector<BONE> mBone;
	}ANIM_MESH;


	typedef struct Bone_Data
	{
		char Bone_Name[MAX_PATH];
		int iParentIndex{};
		XMFLOAT4X4 mTransformation{};
	}BONE_DATA;

	typedef struct Channel
	{
		char mName[MAX_PATH];
		unsigned int mNumScalingKeys{};
		unsigned int mNumRotationKeys{};
		unsigned int mNumPositionKeys{};
		vector<KEYFRAME> mKeyFrame;
	}CHANNEL;

	typedef struct Animation
	{
		char mName[MAX_PATH];
		float mfDuration{};
		float mfTicksPerSecond{};
		unsigned int miNumChannels{};
		vector<CHANNEL> mChannel;
	}ANIMATION;

	typedef struct Anim_Model_Data
	{
		unsigned int mNumMeshes{};
		unsigned int mNumMaterials{};
		unsigned int mNumAnimations{};
		vector<ANIM_MESH> m_Mesh;
		vector<MATERIAL_DATA> Material_Data;
		vector<BONE_DATA> Bone_Data;
		vector<ANIMATION> mAnimation;
	}ANIM_DATA;

	typedef struct Digimon_Info
	{
		wstring DigimonName;
		int DigimonId{};
		wstring DigimonInfo;
		wstring DigimonSkill1Info;
		wstring DigimonSkill2Info;
		wstring DigimonSkill3Info;
		wstring DigimonSkillImage;
		int SkillCount{};
		DIGIMON_STAGE Stage;
		DIGIMON_ATTRIBUTE Attribute;
		int Hp{};
		int Sp{};
		int Damage{};
		int AttackSpeed{};
		int Exp{};
		int Lv{};
	}DIGIMON_INFO;

}

#endif // Engine_Struct_h__
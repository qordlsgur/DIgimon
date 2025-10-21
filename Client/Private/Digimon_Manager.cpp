#include "Digimon_Manager.h"
#include "PartyUHD.h"
#include "Digivice.h"
#include "ContainerObject.h"
#include "GameInstance.h"
#include "Player.h"


IMPLEMENT_SINGLETON(CDigimon_Manager)

CDigimon_Manager::CDigimon_Manager()
{
}

HRESULT CDigimon_Manager::Initialize()
{
	m_iDigivice_Number = 8;

	m_vDigivice_Battle_Slot.resize(m_iDigivice_Number);
	Not_found = L"";

	m_pGameInstance = CGameInstance::GetInstance();

	Model_Name();
	Digimon_Prototypr();
	Digimon_Info_Add();
	//Digimon_Skill_Image_Create();
	return S_OK;
}

void CDigimon_Manager::Digimon_Info_Add()
{
	Angewomon();
	Blackwargreymon();
	Devilmon();
	LedyDevimon();
	Leomon();
	Metalgarumon();
	Metalgeymon();
	Omegamoon();
	Wargreymon();
	Beelzebumon();
}

void CDigimon_Manager::Angewomon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("엔젤우몬");
	Info.DigimonId = 0;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("아름다운 여성의 모습을 한 대천사형 디지몬");
	Info.Damage = 500;

	Info.DigimonSkill1Info.SkillName = TEXT("뺨 때리기");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 1;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 80%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.8f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("상대방의 뺨을 때려 피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("헤븐즈 참");
	Info.DigimonSkill2Info.Sp = 130;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 100%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("아름다운 광선을 발사해\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("홀리 에로우");
	Info.DigimonSkill3Info.Sp = 200;
	Info.DigimonSkill3Info.HitCount = 1;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 130%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 1.3f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("뇌격의 화살을 발사해\n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Angewomon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 5000;
	Info.CurrentHp = 5000;
	Info.Sp = 2500;
	Info.CurrentSp = 2500;
	Info.AttackSpeed = 100;
	Info.Exp = 10000;
	Info.CurrentExp = 0;
	Info.ExpReward = 1000;
	Info.Lv = 50;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Blackwargreymon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("블랙워그레이몬");
	Info.DigimonId = 1;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("칠흑의 용 전사 라고 불리는 두려운 존재인 바이러스종 워그레이몬");
	Info.Damage = 1000;

	Info.DigimonSkill1Info.SkillName = TEXT("드라몬 킬러");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 2;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 40%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.4f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("드라몬 킬러로 찔러\n피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("블랙 토네이도");
	Info.DigimonSkill2Info.Sp = 175;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 100%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("몸을 빠르게 회전시켜 돌격해\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("암흑의 테라 광선");
	Info.DigimonSkill3Info.Sp = 250;
	Info.DigimonSkill3Info.HitCount = 1;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 120%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 1.2f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("부정적인 사념을 모아 압축하여 던져\n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Blackwargreymon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 7000;
	Info.CurrentHp = 7000;
	Info.Sp = 3500;
	Info.CurrentSp = 3500;
	Info.AttackSpeed = 130;
	Info.Exp = 20000;
	Info.CurrentExp = 0;
	Info.ExpReward = 3000;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Devilmon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("데빌몬");
	Info.DigimonId = 2;
	Info.Stage = DIGIMON_STAGE::CHAMPION;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("칠흑의 옷으로 몸을 감싸고 있는 타천사형 디지몬");
	Info.Damage = 300;

	Info.DigimonSkill1Info.SkillName = TEXT("펀치");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 1;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 50%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.5f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("주먹을 이용해 피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("데스 클로");
	Info.DigimonSkill2Info.Sp = 100;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 0.7%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 0.7f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("죽음의 손톱으로 피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("헬 컨트렉트");
	Info.DigimonSkill3Info.Sp = 250;
	Info.DigimonSkill3Info.HitCount = 1;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 100%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 1.f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::ALL;
	Info.DigimonSkill3Info.SkillInfo = TEXT("저주의 말을 퍼부어 피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Devilmon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 2500;
	Info.CurrentHp = 2500;
	Info.Sp = 1250;
	Info.CurrentSp = 1250;
	Info.AttackSpeed = 70;
	Info.Exp = 5000;
	Info.CurrentExp = 0;
	Info.ExpReward = 500;
	Info.Lv = 15;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::LedyDevimon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("레이디데블몬");
	Info.DigimonId = 3;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("고귀한 존재인 여성형 타천사 디지몬");
	Info.Damage = 500;

	Info.DigimonSkill1Info.SkillName = TEXT("다크니스 어텍");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 3;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 20%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.2f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("어둠의 힘이 담긴 연속 공격으로\n피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("뺨 때리기");
	Info.DigimonSkill2Info.Sp = 150;
	Info.DigimonSkill2Info.HitCount = 2;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 50%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 0.5f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("상대방의 뺨을 때려 피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("다크니스 웨이브");
	Info.DigimonSkill3Info.Sp = 250;
	Info.DigimonSkill3Info.HitCount = 4;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 20%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 0.2f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::ALL;
	Info.DigimonSkill3Info.SkillInfo = TEXT("박쥐와 같은 물체을 발사해 \n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Ladydevimon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 5000;
	Info.CurrentHp = 5000;
	Info.Sp = 2500;
	Info.CurrentSp = 2500;
	Info.AttackSpeed = 100;
	Info.Exp = 10000;
	Info.CurrentExp = 0;
	Info.ExpReward = 1000;
	Info.Lv = 50;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Leomon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("레오몬");
	Info.DigimonId = 4;
	Info.Stage = DIGIMON_STAGE::CHAMPION;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("백수의 왕, 고상한 용사라 불리는 수인형 디지몬");
	Info.Damage = 300;

	Info.DigimonSkill1Info.SkillName = TEXT("검휘두르기");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 2;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 20%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.2f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("검을 휘둘러 피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("수왕권");
	Info.DigimonSkill2Info.Sp = 150;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 60%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 0.6f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("사자의 에너지를 발사해\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("수왕파");
	Info.DigimonSkill3Info.Sp = 200;
	Info.DigimonSkill3Info.HitCount = 1;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 80%%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 0.8f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("지면을 뒤틀어 피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Leomon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 2500;
	Info.CurrentHp = 2500;
	Info.Sp = 1250;
	Info.CurrentSp = 1250;
	Info.AttackSpeed = 70;
	Info.Exp = 5000;
	Info.CurrentExp = 0;
	Info.ExpReward = 500;
	Info.Lv = 15;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Metalgarumon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("메탈가루몬");
	Info.DigimonId = 5;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::DATA;
	Info.DigimonInfo = TEXT("전신의 대부분을 메탈화 한 것으로 파워업한 가루몬의 최종형태");
	Info.Damage = 1000;

	Info.DigimonSkill1Info.SkillName = TEXT("강철 발톱");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 1;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 50%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.5f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("앞 발로 할퀴어 피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("가루루 토마호크");
	Info.DigimonSkill2Info.Sp = 150;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 100%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("가슴에서 미사일을 발사해\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("썰렁포");
	Info.DigimonSkill3Info.Sp = 200;
	Info.DigimonSkill3Info.HitCount = 3;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 40%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 0.4f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("입에서 냉기의 브레스를 발사해\n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Metalgarumon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 7000;
	Info.CurrentHp = 7000;
	Info.Sp = 7000;
	Info.CurrentSp = 7000;
	Info.AttackSpeed = 130;
	Info.Exp = 20000;
	Info.CurrentExp = 0;
	Info.ExpReward = 3000;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Metalgeymon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("메탈그레이몬");
	Info.DigimonId = 6;
	Info.Stage = DIGIMON_STAGE::ULTIMATE;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("몸의 절반 이상을 기계화한 사이보그형 디지몬");
	Info.Damage = 500;

	Info.DigimonSkill1Info.SkillName = TEXT("물기");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 1;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 50%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.5f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("상대를 깨물어 피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("기가 디스트로이어");
	Info.DigimonSkill2Info.Sp = 150;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 100%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("미사일을 발사하여 피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("지가 스톰");
	Info.DigimonSkill3Info.Sp = 200;
	Info.DigimonSkill3Info.HitCount = 1;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 120%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 1.2f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("고혈 에너지를 발사해\n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Metalgreymon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 5000;
	Info.CurrentHp = 5000;
	Info.Sp = 2500;
	Info.CurrentSp = 2500;
	Info.AttackSpeed = 100;
	Info.Exp = 10000;
	Info.CurrentExp = 0;
	Info.ExpReward = 1000;
	Info.Lv = 50;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Omegamoon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("오메가몬");
	Info.DigimonId = 7;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("선을 바라는 사람들의 강한 의지에 의해서 융햡해 탄생한 성기사형 디지몬");

	Info.DigimonSkill1Info.SkillName = TEXT("그레이 소드");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 3;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 40%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.4f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("왼 손의 그레이 소드로 공격해\n피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("소드 오브 루인");
	Info.DigimonSkill2Info.Sp = 130;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 120%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.2f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("그레이 소드로 적에게 폭발적인\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("가루루 캐논");
	Info.DigimonSkill3Info.Sp = 250;
	Info.DigimonSkill3Info.HitCount = 3;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 40%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 0.4f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("가루루 캐논으로 공격을 날려\n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Omegamon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 10000;
	Info.CurrentHp = 10000;
	Info.Sp = 5000;
	Info.CurrentSp = 5000;
	Info.Damage = 1300;
	Info.AttackSpeed = 150;
	Info.Exp = 20000;
	Info.CurrentExp = 0;
	Info.ExpReward = 3000;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Wargreymon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("워그레이몬");
	Info.DigimonId = 8;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VACCINE;
	Info.DigimonInfo = TEXT("초금속 크롬디지조이드의 값옷을 입고 있는 최강의 용전사");
	Info.Damage = 1000;

	Info.DigimonSkill1Info.SkillName = TEXT("강철 발톱");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 2;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 50%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.5f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("양 팔의 드라몬 킬러로\n피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("테라 광선");
	Info.DigimonSkill2Info.Sp = 175;
	Info.DigimonSkill2Info.HitCount = 1;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 100%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 1.f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("대기의 기운을 모아 압축하여 던져\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("용기의 방패");
	Info.DigimonSkill3Info.Sp = 200;
	Info.DigimonSkill3Info.HitCount = 1;
	Info.DigimonSkill3Info.Damage = 0;
	Info.DigimonSkill3Info.Target = SKILL_TARGET::SELF;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("용기의 방패를 사용하여 피해를\n경감시킵니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Wargreymon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 7000;
	Info.CurrentHp = 7000;
	Info.Sp = 3500;
	Info.CurrentSp = 3500;
	Info.AttackSpeed = 130;
	Info.Exp = 20000;
	Info.CurrentExp = 0;
	Info.ExpReward = 3000;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Beelzebumon()
{
	Digimon_Info Info;

	Info.DigimonName = TEXT("베르제브몬");
	Info.DigimonId = 9;
	Info.Stage = DIGIMON_STAGE::MEGA;
	Info.Attribute = DIGIMON_ATTRIBUTE::VIRUS;
	Info.DigimonInfo = TEXT("많은 악마형 디지몬을 통괄하는 능력을 가지면서도 고고하게 존재하는 마왕형 디지몬");
	Info.Damage = 1000;

	Info.DigimonSkill1Info.SkillName = TEXT("다크니스 클로");
	Info.DigimonSkill1Info.Sp = 0;
	Info.DigimonSkill1Info.HitCount = 5;
	Info.DigimonSkill1Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 10%");
	Info.DigimonSkill1Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill1Info.HitCount * 0.1f);
	Info.DigimonSkill1Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill1Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill1Info.SkillInfo = TEXT("날카로운 손톱으로 적을 공격해\n피해를 입힙니다.");

	Info.DigimonSkill2Info.SkillName = TEXT("더블 임팩트");
	Info.DigimonSkill2Info.Sp = 200;
	Info.DigimonSkill2Info.HitCount = 7;
	Info.DigimonSkill2Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 15%");
	Info.DigimonSkill2Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill2Info.HitCount * 0.15f);
	Info.DigimonSkill2Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill2Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill2Info.SkillInfo = TEXT("베렌헤나를 빠르게 연사해\n피해를 입힙니다.");

	Info.DigimonSkill3Info.SkillName = TEXT("하트브레이크 샷");
	Info.DigimonSkill3Info.Sp = 250;
	Info.DigimonSkill3Info.HitCount = 2;
	Info.DigimonSkill3Info.DamageInfo = TEXT("기본 데미지 x 공격 횟수 x 50%");
	Info.DigimonSkill3Info.Damage = static_cast<_int>(Info.Damage * Info.DigimonSkill3Info.HitCount * 0.5f);
	Info.DigimonSkill3Info.Target = SKILL_TARGET::ENEMY;
	Info.DigimonSkill3Info.Range = SKILL_RANGE::SINGLE;
	Info.DigimonSkill3Info.SkillInfo = TEXT("베렌헤나로 적의 심장을 공격해\n피해를 입힙니다.");

	Info.DigimonSkillImage = TEXT("Prototype_Component_Texture_Beelzebumon_Skill");
	Info.SkillCount = 3;
	Info.Hp = 7000;
	Info.CurrentHp = 7000;
	Info.Sp = 3500;
	Info.CurrentSp = 3500;
	Info.AttackSpeed = 130;
	Info.Exp = 20000;
	Info.CurrentExp = 0;
	Info.ExpReward = 3000;
	Info.Lv = 90;

	Digimon_Add(Info.DigimonId, Info);
}

void CDigimon_Manager::Swap_Digimon(_int Sour, _int Dest)
{
	if (m_vDigivice_Battle_Slot[Dest] == false)
	{

	}
}

void CDigimon_Manager::Digimon_UHD()
{

}

void CDigimon_Manager::Acquire_Digimon(_int ID)
{
	m_pDigivice->Acquire_Digimon(ID);
}

void CDigimon_Manager::Release_Digimon(_int ID)
{
	m_pDigivice->Release_Digimon(ID);
}

void CDigimon_Manager::Set_Digivice_Slot(_int SlotID, _bool Has, DIGIMON_INFO* Info)
{
	m_vDigivice_Battle_Slot[SlotID] = Has;
	if (SlotID <= 0)
	{
		m_pFirst_Digimon = static_cast<CContainerObject*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), Get_Prototype_ID(Info->DigimonId), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Digimon")));
		static_cast<CPlayer*>(m_pPlayer)->First_Digimon(m_pFirst_Digimon);
	}

	if (SlotID <= 2)
	{
		m_pPartyUHD->Set_Digimon_ID(SlotID, Has, Info);
	}
}

void CDigimon_Manager::Set_Current_Digimon_Info(DIGIMON_INFO* Info)
{
	m_CurrentDigimon.push_back(Info);
}

DIGIMON_INFO* CDigimon_Manager::Get_Current_Digimon_Info(_int Num)
{
	if (m_CurrentDigimon.size() <= Num)
		return nullptr;

	return m_CurrentDigimon[Num];
}

void CDigimon_Manager::Set_Digimon_Update(_int InfoNum, DIGIMON_STATE eState, _int Add)
{
	auto& Digimon = m_CurrentDigimon[InfoNum];

	switch (eState)
	{
	case DIGIMON_STATE::HP:
		Digimon->CurrentHp += Add;
		if (Digimon->CurrentHp > Digimon->Hp)
			Digimon->CurrentHp = Digimon->Hp;

		if (Digimon->CurrentSp < 0)
			Digimon->CurrentSp = 0;
		break;

	case DIGIMON_STATE::SP:
		Digimon->CurrentSp += Add;
		if (Digimon->CurrentSp > Digimon->Sp)
			Digimon->CurrentSp = Digimon->Sp;

		if (Digimon->CurrentSp < 0)
			Digimon->CurrentSp = 0;
		break;

	case DIGIMON_STATE::EXP:
		Digimon->CurrentExp += Add;
		if (Digimon->CurrentExp >= Digimon->Exp)
		{
			while (Digimon->CurrentExp >= Digimon->Exp)
			{
				Digimon->CurrentExp -= Digimon->Exp;
				Digimon->Lv++;
			}
		}
		break;
	}
}

DIGIMON_INFO* CDigimon_Manager::Search_Digimon(_int ID)
{
	auto iter = Find_Digimon(ID);

	if (nullptr == iter)
		return nullptr;

	return iter;
}

const _wstring& CDigimon_Manager::Get_Model_ID(_int ID)
{
	auto iter = Find_Model(ID);

	if (nullptr == iter)
		return Not_found;

	return *iter;
}

const _wstring& CDigimon_Manager::Get_Prototype_ID(_int ID)
{
	auto iter = Find_Prototype(ID);

	if (nullptr == iter)
		return Not_found;

	return *iter;
}

void CDigimon_Manager::PartyUHD(CPartyUHD* pPartyUHD)
{
	m_pPartyUHD = pPartyUHD;
}

void CDigimon_Manager::Digivice(CDigivice* pDigivice)
{
	m_pDigivice = pDigivice;
}

void CDigimon_Manager::Player(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}

_vector CDigimon_Manager::PlayerPos()
{
	return m_pPlayer->Get_Transform();
}

HRESULT CDigimon_Manager::Digimon_Add(_int Digimon_ID, DIGIMON_INFO Info)
{
	if (nullptr != Find_Digimon(Digimon_ID))
		return E_FAIL;

	m_Digimon_Info.emplace(Digimon_ID, Info);

	return S_OK;
}

void CDigimon_Manager::Model_Name()
{
	m_Digimon_Model.emplace(0, TEXT("Prototype_Component_Model_Angewomon"));
	m_Digimon_Model.emplace(1, TEXT("Prototype_Component_Model_Blackwargreymon"));
	m_Digimon_Model.emplace(2, TEXT("Prototype_Component_Model_Devilmon"));
	m_Digimon_Model.emplace(3, TEXT("Prototype_Component_Model_Ladydevimon"));
	m_Digimon_Model.emplace(4, TEXT("Prototype_Component_Model_Leomon"));
	m_Digimon_Model.emplace(5, TEXT("Prototype_Component_Model_Metalgarumon"));
	m_Digimon_Model.emplace(6, TEXT("Prototype_Component_Model_Metalgreymon"));
	m_Digimon_Model.emplace(7, TEXT("Prototype_Component_Model_Omegamon"));
	m_Digimon_Model.emplace(8, TEXT("Prototype_Component_Model_Wargreymon"));
	m_Digimon_Model.emplace(9, TEXT("Prototype_Component_Model_Beelzebumon"));
}

void CDigimon_Manager::Digimon_Prototypr()
{
	m_Digimon_Prototype.emplace(0, TEXT("Prototype_GameObject_Angewomon"));
	m_Digimon_Prototype.emplace(1, TEXT("Prototype_GameObject_Blackwargreymon"));
	m_Digimon_Prototype.emplace(2, TEXT("Prototype_GameObject_Devilmon"));
	m_Digimon_Prototype.emplace(3, TEXT("Prototype_GameObject_Ladydevimon"));
	m_Digimon_Prototype.emplace(4, TEXT("Prototype_GameObject_Leomon"));
	m_Digimon_Prototype.emplace(5, TEXT("Prototype_GameObject_Metalgarumon"));
	m_Digimon_Prototype.emplace(6, TEXT("Prototype_GameObject_Metalgreymon"));
	m_Digimon_Prototype.emplace(7, TEXT("Prototype_GameObject_Omegamon"));
	m_Digimon_Prototype.emplace(8, TEXT("Prototype_GameObject_Wargreymon"));
	m_Digimon_Prototype.emplace(9, TEXT("Prototype_GameObject_Beelzebumon"));
}

DIGIMON_INFO* CDigimon_Manager::Find_Digimon(_int Digimon_ID)
{
	auto iter = m_Digimon_Info.find(Digimon_ID);

	if (iter == m_Digimon_Info.end())
		return nullptr;

	return &iter->second;
}

const _wstring* CDigimon_Manager::Find_Model(_int Digimon_ID)
{
	auto iter = m_Digimon_Model.find(Digimon_ID);

	if (iter == m_Digimon_Model.end())
		return nullptr;

	return &iter->second;
}

const _wstring* CDigimon_Manager::Find_Prototype(_int Digimon_ID)
{
	auto iter = m_Digimon_Prototype.find(Digimon_ID);

	if (iter == m_Digimon_Prototype.end())
		return nullptr;

	return &iter->second;
}

void CDigimon_Manager::Free()
{
	__super::Free();

	m_vDigivice_Battle_Slot.clear();

	Safe_Release(m_pGameInstance);
}

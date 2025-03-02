// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumSet.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	TimerSkill = 0,
	ActiveSkill,
	PassiveSkill
};

UENUM(BlueprintType)
enum class EActiveSkillType : uint8
{
	Null = 0,
	Freezing,
	Slow,
	Mjolnir
};

UENUM(BlueprintType)
enum class EPassiveSkillType : uint8
{
	Null = 0,
	Berserker,
	BloodAbsorbing,
	ElectricChain,
	AmountMaxHp,
	AmountMaxStamina,
	AmountStaminaRecovery
};

UENUM(BlueprintType)
enum class ETimerSkillType : uint8
{
	Null = 0,
	Thunder,
	Meteor,
	AttackPlants,
	HealPlants,
	Plants
};

UENUM(BlueprintType)
enum class ESceneType : uint8
{
	Title = 0,
	Main,
	Mission1,
	Mission2,
	Mission3,
	Infinity,
	Ending
};

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	TitleWidget = 0,
	MainWidget,
	InGameWidget,
	EndingWidget,
	FadeWidget,
	TextWidget,
	OptionWidget,
	ShopWidget,
	ResultWidget,
	CharacterListWidget,
	SkillListWidget,
	MissionListWidget
};

UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	Player = 0,
	Monster,
	Npc,
	Item
};

UENUM(BlueprintType)
enum class ESoundType : uint8
{
	Attack = 0,
	Hit,
	Die,
	MeleeAttack,
	Reload,
	Respawn,
	Avoid
};

UENUM(BlueprintType)
enum class EBGMSoundType : uint8
{
	Title = 0,
	Main,
	Mission1,
	Mission2,
	Mission3,
	Infinity,
	Ending,
	Defence,
	Boss,
	Intro
};

UENUM(BlueprintType)
enum class EUISoundType : uint8
{
	Open = 0,
	Click
};

UENUM(BlueprintType)
enum class ETableType : uint8
{
	PlayerSkill = 0,
	ShopCatalog,
	CharacterInfo,
	PlayData,
	MissionItem
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Gunner = 0,
	Aurora,
	Fey,
	Sparrow
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle = 0,
	Move,
	Reload,
	Attack,
	Hit,
	Die
};

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Melee = 0,
	Ranger,
	Suicide,
	Boss
};

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle = 0,
	Patrol,
	Trace,
	Attack,
	Hit,
	Die
};

UENUM(BlueprintType)
enum class ENpcType : uint8
{
	Baldor = 0
};

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Idle = 0,
	Trace,
	Attack,
	Hit,
	Die
};

UENUM(BlueprintType)
enum class EBossState : uint8 {
	Idle,
	Chase,
	Attack1,
	Attack2,
	Attack3,
	Attack4,
	Skill1,
	Skill2,
	Skill3,
	Dead
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Gold = 0,
	HealKit
};

UCLASS()
class EDMUNDPRJ_API UEnumSet : public UObject
{
	GENERATED_BODY()
	
};

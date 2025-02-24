// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumSet.generated.h"

UENUM(BlueprintType)
enum class ETimerSkillType : uint8
{
	Thunder = 0,
	Meteor,
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
enum class ESoundType : uint8
{
	BGM = 0,
	UI,
	Player,
	Monster,
	Item,
	Etc
};

UENUM(BlueprintType)
enum class ETableType : uint8
{
	PlayerSkill = 0,
	ShopCatalog,
	MissionItem
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Gunner = 0,
	Other1,
	Other2,
	Other3
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
enum class EMonsterState : uint8
{
	Idle = 0,
	Patrol,
	Trace,
	Attack,
	Hit,
	Die
};

UCLASS()
class EDMUNDPRJ_API UEnumSet : public UObject
{
	GENERATED_BODY()
	
};

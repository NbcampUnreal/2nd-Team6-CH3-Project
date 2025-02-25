// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameMode.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameState.h"
#include "System/MissionHandle.h"

void AEdmundGameMode::BeginPlay()
{
	Super::BeginPlay();

	EdmundGameInstance = GetGameInstance<UEdmundGameInstance>();
	EdmundGameState = Cast<AEdmundGameState>(GameState);

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	checkf(IsValid(EdmundGameState), TEXT("GameState is invalid"));

	CurrentScene = EdmundGameInstance->GetCurrentSceneName();

	InitMission();
}

void AEdmundGameMode::InitDefaultPawnByCharacterType()
{
	if (EdmundGameInstance->GetPlayerType() == ECharacterType::Gunner)
	{
		DefaultPawnClass = GunnerClass;
		PlayerControllerClass = TestPlayerController;
	}
	// 찾아보고 해야할듯
}

void AEdmundGameMode::InitMission()
{
	MissionHandle = GetWorld()->SpawnActor<AMissionHandle>(MissionHandleClass);
	
	switch (CurrentScene)
	{
	case ESceneType::Title:
		return;

	case ESceneType::Main:
		return;

	case ESceneType::Ending:
		return;

	default:
		MissionHandle->InitMissionHandle(EdmundGameInstance->GetCurrentMissionData(CurrentScene), this, EdmundGameState);
	}
}

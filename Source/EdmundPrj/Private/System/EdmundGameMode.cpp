// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameMode.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameState.h"
#include "System/MissionHandle.h"

void AEdmundGameMode::RequestInteractionToMissionHandle()
{
	checkf(IsValid(MissionHandle), TEXT("Mission Handle is invalid"));
	MissionHandle->OnPressedKeyFromPlayer();
}

void AEdmundGameMode::EndMission()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->EndMission();
}

void AEdmundGameMode::BeginPlay()
{
	Super::BeginPlay();

	EdmundGameInstance = GetGameInstance<UEdmundGameInstance>();
	EdmundGameState = GetGameState<AEdmundGameState>();
	
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	checkf(IsValid(EdmundGameState), TEXT("GameState is invalid"));

	CurrentScene = EdmundGameInstance->GetCurrentSceneName();
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
	switch (CurrentScene)
	{
	case ESceneType::Title:
		return;

	case ESceneType::Main:
		return;

	case ESceneType::Ending:
		return;

	default:
		MissionHandle = GetWorld()->SpawnActor<AMissionHandle>(MissionHandleClass);
		MissionHandle->InitMissionHandle(EdmundGameInstance->GetCurrentMissionData(CurrentScene), this, EdmundGameState);
	}
}

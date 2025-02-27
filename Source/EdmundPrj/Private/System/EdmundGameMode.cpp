// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameMode.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameState.h"
#include "System/MissionHandle.h"
#include "System/SpawnerHandle.h"


void AEdmundGameMode::InitGameMode(UEdmundGameInstance* NewGameInstance, const TArray<FMissionDataRow*>& MissionDataSet, const TArray<FSpawnerDataRow*>& SpawnerDataSet)
{
	EdmundGameInstance = NewGameInstance;
	EdmundGameState = Cast<AEdmundGameState>(GameState);

	checkf(IsValid(MissionHandleClass), TEXT("Mission Handle Class is invalid"));
	MissionHandle = GetWorld()->SpawnActor<AMissionHandle>(MissionHandleClass);
	MissionHandle->InitMissionHandle(MissionDataSet, this, EdmundGameState);
	EdmundGameState->SetMissionHandle(MissionHandle);

	checkf(IsValid(SpawnerHandleClass), TEXT("Spawner Handle Class is invalid"));
	SpawnerHandle = GetWorld()->SpawnActor<ASpawnerHandle>(SpawnerHandleClass);
	SpawnerHandle->InitSpawnerHandle(this, EdmundGameState, SpawnerDataSet);
	EdmundGameState->SetSpawnerHandle(SpawnerHandle);
}

void AEdmundGameMode::ClearMission()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->EndMission(true);
}

void AEdmundGameMode::FailMission()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->EndMission(false);
}

void AEdmundGameMode::BeginPlay()
{
	Super::BeginPlay();
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

void AEdmundGameMode::StartMission(ESceneType CurrentScene)
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
		MissionHandle->ApplyMissionDataInLevel();
		SpawnerHandle->ApplySpawnerDataInLevel();
	}
}


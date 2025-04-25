// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameMode.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameState.h"
#include "System/MissionHandle.h"
#include "System/SpawnerHandle.h"
#include "Player/BaseCharacter.h"
#include "System/DataStructure/StoryDataRow.h"

void AEdmundGameMode::InitGameMode(
	UEdmundGameInstance* NewGameInstance, 
	const TArray<FMissionDataRow*>& MissionDataSet, 
	const TArray<FSpawnerDataRow*>& SpawnerDataSet,
	const TArray<FStoryDataRow*>& StoryDataSet,
	UClass* CharacterClass
)
{
	EdmundGameInstance = NewGameInstance;
	EdmundGameState = Cast<AEdmundGameState>(GameState);

	MissionStoryData = StoryDataSet;

	checkf(IsValid(MissionHandleClass), TEXT("Mission Handle Class is invalid"));
	MissionHandle = GetWorld()->SpawnActor<AMissionHandle>(MissionHandleClass);
	MissionHandle->InitMissionHandle(MissionDataSet, this, EdmundGameState);
	EdmundGameState->SetMissionHandle(MissionHandle);

	checkf(IsValid(SpawnerHandleClass), TEXT("Spawner Handle Class is invalid"));
	SpawnerHandle = GetWorld()->SpawnActor<ASpawnerHandle>(SpawnerHandleClass);
	SpawnerHandle->InitSpawnerHandle(this, EdmundGameState, SpawnerDataSet);
	EdmundGameState->SetSpawnerHandle(SpawnerHandle);

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	UEnum* CharacterEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterType"));
	ECharacterType CharacterType = EdmundGameInstance->GetPlayerType();
	FString TypeName = CharacterEnum->GetNameStringByValue((int64)CharacterType);

	if (OptionsString.Contains(TypeName))
	{
		SpawnPlayerByCharacterType(CharacterClass);
	}
	
}

void AEdmundGameMode::SpawnPlayerByCharacterType(UClass* SpawnClass)
{
	checkf(IsValid(SpawnClass), TEXT("Target Class is invalid"));
	
	APlayerController* PlayerController = EdmundGameState->GetPlayerController();
	FActorSpawnParameters SpawnParam;
	FVector StartPos = FindPlayerStart(PlayerController)->GetActorLocation();

	ABaseCharacter* PlayerCharacter = GetWorld()->SpawnActor<ABaseCharacter>(SpawnClass, StartPos, FRotator::ZeroRotator, SpawnParam);
	
	PlayerController->Possess(PlayerCharacter);
	PlayerCharacter->PossessedBy(PlayerController);

	EdmundGameState->SetPlayerPawn(PlayerCharacter);
}

void AEdmundGameMode::ClearMission()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->EndMission(true);
	EdmundGameState->EndCurrentLevel(true);
}

void AEdmundGameMode::FailMission()
{
	if (bIsCleared)
	{
		return;
	}
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->EndMission(false);
	EdmundGameState->EndCurrentLevel(false);
}

void AEdmundGameMode::NotifyAllClearedMission()
{
	bIsCleared = true;

	if (!bIsPlayingStory)
	{
		ClearMission();
	}
}

void AEdmundGameMode::OnStartedPrintStory(const int32 Index)
{
	CurrentStoryData = nullptr;

	if (Index < MissionStoryData.Num())
	{
		for (FStoryDataRow* StoryDataRow : MissionStoryData)
		{
			if (StoryDataRow->Index == Index)
			{
				CurrentStoryData = StoryDataRow;
				break;
			}
		}

		checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
		EdmundGameInstance->VisibleMissionStory();
		CurrentStoryIndex = 0;
		bIsPlayingStory = true;
		CheckRemainCurrentStory();
	}
}

void AEdmundGameMode::PrintCurrentStory()
{
	EdmundGameState->PrintStoryText(CurrentStoryData->StoryText[CurrentStoryIndex]);
	++CurrentStoryIndex;
}

bool AEdmundGameMode::CheckRemainCurrentStory()
{
	if (CurrentStoryIndex < CurrentStoryData->StoryText.Num())
	{
		PrintCurrentStory();
		return true;
	}
	else
	{
		OnEndedCurrentStory();
		return false;
	}
}

void AEdmundGameMode::OnEndedCurrentStory()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->InvisibleMissionStory();
	bIsPlayingStory = false;

	if (bIsCleared)
	{
		ClearMission();
	}
}

void AEdmundGameMode::StartDefenceMode()
{
	SpawnerHandle->ApplyDefenceMode();
}

void AEdmundGameMode::StartBossMission()
{
	SpawnerHandle->DestroyAllSpawner();
	EdmundGameState->NotifyOnStageProgress("Boss", true);
}

void AEdmundGameMode::SpawnMonsterByBoss(const TArray<FVector>& ActiveDimensionPosSet)
{
	SpawnerHandle->SpawnBossPatternSpawner(ActiveDimensionPosSet);
}

void AEdmundGameMode::NotifyClearedHalfPatternByBoss()
{
	MissionHandle->ApplyNextPatternFromHalf();
}

void AEdmundGameMode::SwapBgm(EBGMSoundType Type)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->PlayBGM(Type);
}

void AEdmundGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AEdmundGameMode::StartMission(ESceneType CurrentScene)
{
	switch (CurrentScene)
	{
	case ESceneType::Title:
		return;

	case ESceneType::Main:
		EdmundGameState->InitMainLevelPlayerController();
		return;

	case ESceneType::Ending:
		return;

	default:
		MissionHandle->ApplyMissionDataInLevel();
		SpawnerHandle->ApplySpawnerDataInLevel();
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameState.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameMode.h"
#include "System/MainLevelPlayerController.h"
#include "System/DataStructure/PlayerSkillRow.h"
#include "System/Observer/GameStateObserver.h"
#include "Player/BaseCharacter.h"
#include "System/MissionHandle.h"
#include "System/SpawnerHandle.h"

void AEdmundGameState::BeginPlay()
{
	Super::BeginPlay();

	EdmundGameInstance = GetWorld()->GetGameInstance<UEdmundGameInstance>();
	EdmundGameMode = GetWorld()->GetAuthGameMode<AEdmundGameMode>();
	PlayerController = GetWorld()->GetPlayerControllerIterator()->Get();

	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	PlayerPawn = PlayerController->GetPawn();

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestGameStart(EdmundGameMode, this);

	InitSkillData();
	InitMainLevel();
}

void AEdmundGameState::BeginDestroy()
{
	Super::BeginDestroy();
}

void AEdmundGameState::ChangeCursorMode(bool bIsValid)
{
	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	PlayerController->bShowMouseCursor = bIsValid;
}

void AEdmundGameState::ChangeInputMode(const FInputModeDataBase& InputMode)
{
	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	PlayerController->SetInputMode(InputMode);
	PlayerController->FlushPressedKeys();
}

void AEdmundGameState::AddCurrentLevelMoney(int32 Money)
{
	if (Money < 0)
	{
		return;
	}

	CurrentLevelMoney += Money;
}

void AEdmundGameState::InitMainLevel()
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	ESceneType CurrentScene = EdmundGameInstance->GetCurrentSceneName();

	if (CurrentScene != ESceneType::Main)
	{
		return;
	}

	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));
	MainLevelPlayerController->InitMainLevelCharacters(EdmundGameInstance->GetCharacterData());
}

void AEdmundGameState::InitSkillData()
{
	SkillDataSet = EdmundGameInstance->GetPlayerSkillData();

	for (FPlayerSkillRow* PlayerSkillRow : SkillDataSet)
	{
		CurrentSkillMap.Add({ PlayerSkillRow, 0 });
	}

	RandomSkillSet.Empty();
}

void AEdmundGameState::CalculateSkillList() // 성능 개선 필요
{
	int32 RandomNum = FMath::RandRange(0, SkillDataSet.Num() - 1);

	FPlayerSkillRow* RandomSkill = SkillDataSet[RandomNum];

	if (RandomSkillSet.Contains(RandomSkill))
	{
		CalculateSkillList();
	}
	else
	{
		if (RandomSkill->MaxLevel == CurrentSkillMap[RandomSkill])
		{
			CalculateSkillList();
		}
		else
		{
			RandomSkillSet.Add(RandomSkill);
		}
	}
}

void AEdmundGameState::CreateRandomSkillSet()
{
	RandomSkillSet.Empty();

	for (int32 i = 0; i < 3; i++)
	{
		CalculateSkillList();
	}

	NotifyCreateRandomSkill();
	EdmundGameInstance->OnSkillListUI();
}

const TArray<FPlayerSkillRow*>& AEdmundGameState::GetRandomSkillSet() const
{
	return RandomSkillSet;
}

void AEdmundGameState::ApplySelectedSkill(const int32 Index)
{
	if (Index >= RandomSkillSet.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Skill Index is invalid"));
	}

	FPlayerSkillRow* SelectedSkill = RandomSkillSet[Index];
	
	++CurrentSkillMap[SelectedSkill];

	UE_LOG(LogTemp, Warning, TEXT("Selected Skill Name is %s"), *SelectedSkill->SkillName.ToString());
	// 플레이어에 전달 필요
}

void AEdmundGameState::SetSelectedCharacter(AActor* Character)
{
	checkf(IsValid(Character), TEXT("Selected Character is invalid"));

	ABaseCharacter* TargetActor = Cast<ABaseCharacter>(Character);
	NotifySelectCharacterType(TargetActor->GetCharacterType());
}

void AEdmundGameState::CancleSelectedCharacter()
{
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));
	MainLevelPlayerController->SetTargetToNull();
}

void AEdmundGameState::SetMissionHandle(AMissionHandle* NewMissionHandle)
{
	MissionHandle = NewMissionHandle;
}

void AEdmundGameState::SetSpawnerHandle(ASpawnerHandle* NewSpawnerHandle)
{
	SpawnerHandle = NewSpawnerHandle;
}

void AEdmundGameState::OnPressedPauseKey()
{
	checkf(IsValid(EdmundGameInstance), TEXT("Game Instance is invalid"));
	EdmundGameInstance->OnPause();
	PlayerController->SetPause(true);
	ChangeCursorMode(true);
	ChangeInputMode(FInputModeUIOnly());
}

void AEdmundGameState::RequestEndPause()
{
	PlayerController->SetPause(false);
}

void AEdmundGameState::RequestInteraction()
{
	checkf(IsValid(MissionHandle), TEXT("Mission Handle is invalid"));
	MissionHandle->OnPressedKeyFromPlayer();
}

void AEdmundGameState::EndCurrentLevel()
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->AddPossessMoney(CurrentLevelMoney);
}

void AEdmundGameState::RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer)
{
	Observers.Add(Observer);
}

void AEdmundGameState::UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer)
{
	Observers.Remove(Observer);
}

void AEdmundGameState::NotifyUpdateNotifyText(const FString& NotifyText)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedNotifyText(NotifyText);
	}
}

void AEdmundGameState::NotifyUpdateMissionText(const FString& MissionText)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedMissionText(MissionText);
	}
}

void AEdmundGameState::NotifyPlayerHp(const int32 MaxHp, const int32 CurrentHp)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerHp(MaxHp, CurrentHp);
	}

	if (CurrentHp <= 0)
	{
		EdmundGameMode->FailMission();
	}
}

void AEdmundGameState::NotifyPlayerOther(const int32 MaxValue, const int32 CurrentValue)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerOther(MaxValue, CurrentValue);
	}
}

void AEdmundGameState::NotifyPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
}

APlayerController* AEdmundGameState::GetPlayerController()
{
	return PlayerController;
}

AActor* AEdmundGameState::GetPlayerPawn()
{
	return PlayerPawn;
}

void AEdmundGameState::NotifyCreateRandomSkill() const
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedSkillList();
	}
}

void AEdmundGameState::NotifySelectCharacterType(ECharacterType CharacterType) const
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedCharacterType(CharacterType);
	}
}

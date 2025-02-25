// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameState.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameMode.h"
#include "System/MainLevelPlayerController.h"
#include "System/DataStructure/PlayerSkillRow.h"
#include "System/Observer/GameStateObserver.h"
#include "Player/BaseCharacter.h"

void AEdmundGameState::BeginPlay()
{
	Super::BeginPlay();

	EdmundGameInstance = GetWorld()->GetGameInstance<UEdmundGameInstance>();
	EdmundGameMode = GetWorld()->GetAuthGameMode<AEdmundGameMode>();
	PlayerController = GetWorld()->GetPlayerControllerIterator()->Get();

	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	PlayerPawn = PlayerController->GetPawn();

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->StartedGameState();

	InitSkillData();
	InitMainLevel();

	//Test
	/*GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::EndCurrentMission, 5.0f, false);*/
}

void AEdmundGameState::BeginDestroy()
{
	Super::BeginDestroy();
}

void AEdmundGameState::EndCurrentMission()
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Title)
	{
		return;
	}
	else if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}
	CreateRandomSkillSet();
	EdmundGameInstance->OnSkillListUI();
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
	NotifySelectCharacterType(TargetActor->getCharacterType());
}

void AEdmundGameState::CancleSelectedCharacter()
{
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));
	MainLevelPlayerController->SetTargetToNull();
}

void AEdmundGameState::RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer)
{
	Observers.Add(Observer);
}

void AEdmundGameState::UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer)
{
	Observers.Remove(Observer);
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

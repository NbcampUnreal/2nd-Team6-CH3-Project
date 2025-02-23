// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameState.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameMode.h"
#include "System/MainLevelPlayerController.h"

void AEdmundGameState::BeginPlay()
{
	Super::BeginPlay();

	EdmundGameInstance = GetWorld()->GetGameInstance<UEdmundGameInstance>();
	EdmundGameMode = GetWorld()->GetAuthGameMode<AEdmundGameMode>();
	PlayerController = GetWorld()->GetPlayerControllerIterator()->Get();

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));

	EdmundGameInstance->StartedGameState();

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::EndCurrentMission, 5.0f, false);
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
	EdmundGameInstance->EndMission();
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

void AEdmundGameState::OnClickedCharacter(ECharacterType CharacterType)
{
	//observer notify ±¸Çö
}

void AEdmundGameState::ChangeSelectMode(const bool bIsSelect) const
{
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));

	//MainLevelPlayerController->
}

void AEdmundGameState::CalculateSkillList()
{

}

void AEdmundGameState::ApplySelectedSkill(const int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Selected Skill Index is %d"), Index);
}

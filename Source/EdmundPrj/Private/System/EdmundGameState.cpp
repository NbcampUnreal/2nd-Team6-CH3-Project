// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameState.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameMode.h"

void AEdmundGameState::BeginPlay()
{
	Super::BeginPlay();
	
	EdmundGameInstance = GetWorld()->GetGameInstance<UEdmundGameInstance>();
	EdmundGameMode = GetWorld()->GetAuthGameMode<AEdmundGameMode>();
	PlayerController = GetWorld()->GetPlayerControllerIterator()->Get();

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));

	EdmundGameInstance->StartedGameState();
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
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MainLevelPlayerController.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "System/EdmundGameState.h"

void AMainLevelPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (IsValid(LocalPlayer))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MainLevelIMC)
			{
				Subsystem->AddMappingContext(MainLevelIMC, 0);
			}
		}
	}
}

void AMainLevelPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	checkf(IsValid(EnhancedInputComponent), TEXT("EnhancedInputComponent is invalid"));
	checkf(IsValid(LeftClickAction), TEXT("LeftClickAction is invalid"));

	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &ThisClass::InputLeftMouseButtonClick);
}

void AMainLevelPlayerController::InputLeftMouseButtonClick()
{
	CheckCollideCharacter();
}

void AMainLevelPlayerController::CheckCollideCharacter()
{
	AActor* TargetActor = nullptr;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (!Hit.bBlockingHit)
	{
		return;
	}

	if (!Hit.GetActor()->ActorHasTag("Player"))
	{
		return;
	}

	TargetActor = Hit.GetActor();
	UE_LOG(LogTemp, Warning, TEXT("Target Character Name is %s"), *TargetActor->GetName());

	AEdmundGameState* EdmundGameState = GetWorld()->GetGameState<AEdmundGameState>();
	checkf(IsValid(EdmundGameState), TEXT("GameState is invalid"));

	//EdmundGameState->OnClickedCharacter();
}

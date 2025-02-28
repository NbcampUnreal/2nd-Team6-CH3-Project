// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MainLevelPlayerController.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "System/EdmundGameState.h"
#include "Player/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "System/DataStructure/CharacterDataRow.h"

AMainLevelPlayerController::AMainLevelPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

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

void AMainLevelPlayerController::Tick(float DelTime)
{
	MoveToTargetPoint();
	MoveToStartPoint();
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
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, false, Hit);

	if (!Hit.bBlockingHit)
	{
		return;
	}

	if (!Hit.GetActor()->ActorHasTag("Player"))
	{
		return;
	}

	if (!bIsSelectMode)
	{
		return;
	}

	bIsSelectMode = false;

	if (IsValid(TargetCharacter))
	{
		bIsReturnByCharacter[TargetCharacter] = true;
	}

	TargetCharacter = Cast<ABaseCharacter>(Hit.GetActor());
	UE_LOG(LogTemp, Warning, TEXT("Target Character Name is %s"), *TargetCharacter->GetName());

	checkf(IsValid(EdmundGameState), TEXT("GameState is invalid"));
	EdmundGameState->SetSelectedCharacter(TargetCharacter);
}

void AMainLevelPlayerController::MoveToTargetPoint()
{
	if (IsValid(TargetCharacter))
	{
		if (bIsReturnByCharacter[TargetCharacter])
		{
			bIsReturnByCharacter[TargetCharacter] = false;
		}

		if (150 >= FVector::Distance(MoveTargetPos, TargetCharacter->GetActorLocation()))
		{
			FVector CurrentTarget = TargetCharacter->GetActorLocation();
			CurrentTarget = FVector(CurrentTarget.X, CurrentTarget.Y, 0);
			FVector LookForwardDirection = LookAtTargetPos - CurrentTarget;
			LookForwardDirection = LookForwardDirection / LookForwardDirection.Size();
			FRotator LookRotator(0, LookForwardDirection.Rotation().Yaw, 0);
			TargetCharacter->SetActorRotation(LookRotator);
			return;
		}

		FVector Direction = MoveTargetPos - StartPosByCharacter[TargetCharacter];
		Direction = Direction / Direction.Size();
		FRotator NewLookRotator(0, Direction.Rotation().Yaw, 0);

		TargetCharacter->SetActorRotation(NewLookRotator);
		TargetCharacter->AddMovementInput(Direction, 0.3f);
	}
}

void AMainLevelPlayerController::MoveToStartPoint()
{
	for (TPair<TObjectPtr<ABaseCharacter>, FVector>& BaseCharacter : StartPosByCharacter)
	{
		if (!bIsReturnByCharacter[BaseCharacter.Key])
		{
			continue;
		}

		FVector Direction = BaseCharacter.Value - BaseCharacter.Key->GetActorLocation();
		Direction = Direction / Direction.Size();
		FRotator NewLookAt(0, Direction.Rotation().Yaw, 0);

		BaseCharacter.Key->SetActorRotation(NewLookAt);
		BaseCharacter.Key->AddMovementInput(Direction, 0.3f);

		if (FVector::Distance(BaseCharacter.Value, BaseCharacter.Key->GetActorLocation()) <= 150)
		{
			bIsReturnByCharacter[BaseCharacter.Key] = false;
			BaseCharacter.Key->SetActorLocation(BaseCharacter.Value);

			FVector LookAtDirection = LookAtTargetPos - BaseCharacter.Value;
			LookAtDirection = LookAtDirection / LookAtDirection.Size();
			FRotator LookAtRotator(0, LookAtDirection.Rotation().Yaw, 0);
			BaseCharacter.Key->SetActorRotation(LookAtRotator);
		}
	}
}

void AMainLevelPlayerController::SetTargetToNull()
{
	bIsReturnByCharacter[TargetCharacter] = true;
	TargetCharacter = nullptr;
	bIsSelectMode = true;
}

void AMainLevelPlayerController::SetSelectMode(bool Value)
{
	bIsSelectMode = Value;
}

void AMainLevelPlayerController::InitMainLevelCharacters(const TArray<FCharacterDataRow*>& CharacterData, ECharacterType Type, AEdmundGameState* NewGameState)
{
	EdmundGameState = NewGameState;
	MoveTargetPos = GetPawn()->GetActorLocation();
	MoveTargetPos = FVector(MoveTargetPos.X, MoveTargetPos.Y, 0);
	LookAtTargetPos = GetPawn()->GetComponentByClass<UCameraComponent>()->GetComponentLocation();
	LookAtTargetPos = FVector(LookAtTargetPos.X, LookAtTargetPos.Y, 0);

	for (FCharacterDataRow* CharacterDataRow : CharacterData)
	{
		UClass* SpawnClass = CharacterDataRow->CharacterClass.Get();
		checkf(IsValid(SpawnClass), TEXT("Spawn Class is invalid"));

		FVector SpawnPos = CharacterDataRow->SpawnLocation;
		FActorSpawnParameters SpawnParam;

		ABaseCharacter* NewCharacter = GetWorld()->SpawnActor<ABaseCharacter>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

		SpawnPos = FVector(SpawnPos.X, SpawnPos.Y, 0);
		StartPosByCharacter.Add(NewCharacter, SpawnPos);
		bIsReturnByCharacter.Add(NewCharacter, false);
		CharacterSet.Add(NewCharacter);

		FVector LookAtDirection = LookAtTargetPos - SpawnPos;
		LookAtDirection = LookAtDirection / LookAtDirection.Size();
		FRotator LookAtRotator(0, LookAtDirection.Rotation().Yaw, 0);
		NewCharacter->SetActorRotation(LookAtRotator);
		NewCharacter->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

		if (!IsValid(TargetCharacter))
		{
			if (CharacterDataRow->CharacterType == Type)
			{
				TargetCharacter = NewCharacter;
				EdmundGameState->SetSelectedCharacter(TargetCharacter);
			}
		}
	}
}

void AMainLevelPlayerController::CompareType(ECharacterType Type)
{
	if (TargetCharacter != nullptr)
	{
		return;
	}

	for (ABaseCharacter* BaseCharacter : CharacterSet)
	{
		if (BaseCharacter->GetCharacterType() == Type)
		{
			TargetCharacter = BaseCharacter;
			return;
		}
	}
}

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
	if (!IsValid(TargetCharacter))
	{
		return;
	}

	if (bIsReturnByCharacter[TargetCharacter])
	{
		bIsReturnByCharacter[TargetCharacter] = false;
	}

	float MoveDistance = DistanceByTarget[TargetCharacter].Size();
	FVector CurrentPos(TargetCharacter->GetActorLocation().X, TargetCharacter->GetActorLocation().Y, 0);
	FRotator MoveLookAt(0, DistanceByTarget[TargetCharacter].Rotation().Yaw, 0);
	
	if (FVector::Dist(CurrentPos, StartPosByCharacter[TargetCharacter]) >= MoveDistance)
	{
		FVector LookVector = LookAtTargetPos - CurrentPos;
		FRotator CameraLookAt(0, LookVector.Rotation().Yaw, 0);
		TargetCharacter->SetActorRotation(CameraLookAt);
		return;
	}

	TargetCharacter->AddMovementInput(DistanceByTarget[TargetCharacter], 0.3f);
	TargetCharacter->SetActorRotation(MoveLookAt);
}

void AMainLevelPlayerController::MoveToStartPoint()
{
	for (TPair<TObjectPtr<ABaseCharacter>, FVector>& Pair : DistanceByTarget)
	{
		if (!bIsReturnByCharacter[Pair.Key])
		{
			continue;
		}

		float Distance = Pair.Value.Size();
		FRotator MoveLookAt(0, Pair.Value.Rotation().Yaw + 180, 0);
		FVector CurrentPos(Pair.Key->GetActorLocation().X, Pair.Key->GetActorLocation().Y, 0);

		if (FVector::Dist(CurrentPos, MoveTargetPos) >= Distance)
		{
			bIsReturnByCharacter[Pair.Key] = false;

			FVector LookVector = LookAtTargetPos - CurrentPos;
			FRotator CameraLookAt(0, LookVector.Rotation().Yaw, 0);
			Pair.Key->SetActorRotation(CameraLookAt);
			continue;
		}

		Pair.Key->SetActorRotation(MoveLookAt);
		Pair.Key->AddMovementInput(-Pair.Value, 0.3f);
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

		FActorSpawnParameters SpawnParam;

		FVector SpawnPos = CharacterDataRow->SpawnLocation;

		ABaseCharacter* NewCharacter = GetWorld()->SpawnActor<ABaseCharacter>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

		SpawnPos = FVector(SpawnPos.X, SpawnPos.Y, 0);
		FVector MoveDirection = MoveTargetPos - SpawnPos;
		FVector LookAtDirection = LookAtTargetPos - SpawnPos;

		StartPosByCharacter.Add(NewCharacter, SpawnPos);
		DistanceByTarget.Add(NewCharacter, MoveDirection);
		bIsReturnByCharacter.Add(NewCharacter, false);
		CharacterSet.Add(NewCharacter);

		FRotator LookAtRotator(0, LookAtDirection.Rotation().Yaw, 0);
		NewCharacter->SetActorRotation(LookAtRotator);
		NewCharacter->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

		if (!IsValid(TargetCharacter))
		{
			if (CharacterDataRow->CharacterType == Type)
			{
				TargetCharacter = NewCharacter;
				EdmundGameState->SetSelectedCharacter(TargetCharacter);
				bIsSelectMode = false;
			}
		}
	}
}

void AMainLevelPlayerController::CompareType(ECharacterType Type)
{
	if (IsValid(TargetCharacter))
	{
		SetTargetToNull();
	}

	for (ABaseCharacter* BaseCharacter : CharacterSet)
	{
		if (BaseCharacter->GetCharacterType() == Type)
		{
			TargetCharacter = BaseCharacter;
			EdmundGameState->SetSelectedCharacter(TargetCharacter);
			bIsSelectMode = false;
			return;
		}
	}
}

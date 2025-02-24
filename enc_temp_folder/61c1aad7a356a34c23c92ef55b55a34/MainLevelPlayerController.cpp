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

	TargetCharacter = Cast<ABaseCharacter>(Hit.GetActor());
	UE_LOG(LogTemp, Warning, TEXT("Target Character Name is %s"), *TargetCharacter->GetName());

	AEdmundGameState* EdmundGameState = GetWorld()->GetGameState<AEdmundGameState>();
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

		FVector Direction = MoveTargetPos - StartPosByCharacter[TargetCharacter];
		Direction = Direction / Direction.Size();
		FRotator NewLookRotator(0, Direction.Rotation().Yaw, 0);

		TargetCharacter->SetActorRotation(NewLookRotator);
		TargetCharacter->AddMovementInput(Direction, 0.3f);
		
		FVector CharacterPos = TargetCharacter->GetActorLocation();
		CharacterPos = FVector(CharacterPos.X, CharacterPos.Y, 0);

		if (30 >= FVector::Distance(MoveTargetPos, CharacterPos))
		{
			UE_LOG(LogTemp, Warning, TEXT("In Near"));
			FVector LookForwardDirection = LookAtTargetPos - TargetCharacter->GetActorLocation();
			LookForwardDirection = LookForwardDirection / LookForwardDirection.Size();
			FRotator LookRotator(0, LookForwardDirection.Rotation().Yaw, 0);
			TargetCharacter->SetActorRotation(LookRotator);
		}
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

		if (FVector::Distance(BaseCharacter.Value, BaseCharacter.Key->GetActorLocation()) <= 50)
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

void AMainLevelPlayerController::InitMainLevelCharacters(const TArray<FCharacterDataRow*>& CharacterData)
{
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
	}

	//TargetCharacter = CharacterSet[0]; // 나중에 수정 필요
}

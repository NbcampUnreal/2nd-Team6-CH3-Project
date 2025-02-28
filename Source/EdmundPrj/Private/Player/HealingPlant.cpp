// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HealingPlant.h"
#include "Player\BaseCharacter.h"
#include "Components\SphereComponent.h"
AHealingPlant::AHealingPlant()
{
	CheckCharacterCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CheckCharacterCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealingPlant::BeginOverlapCharacter);
	CheckCharacterCollision->OnComponentEndOverlap.AddDynamic(this, &AHealingPlant::EndOverlapCharacter);
}

void AHealingPlant::BeginOverlapCharacter(
	UPrimitiveComponent* overlappedComp,
	AActor* otherActor,
	UPrimitiveComponent* otherComp,
	int32 otherBodyIndex,
	bool bFromSweep, const
	FHitResult& SweepResult)
{
	if (otherActor && otherActor->ActorHasTag("Player"))
	{
		if (TObjectPtr<ABaseCharacter> character = Cast<ABaseCharacter>(otherActor))
		{
			Character = character;
		}
	}
}

void AHealingPlant::EndOverlapCharacter(UPrimitiveComponent*
	overlappedComponent,
	AActor* otherActor,
	UPrimitiveComponent* otherComp,
	int32 otherBodyIndex)
{
	if (otherActor && otherActor->ActorHasTag("Player"))
	{
		Character = nullptr;
	}
}


void AHealingPlant::HealingCharacter()
{
	if (Character != nullptr)
	{
		Character->AmountHP(HealAmount);
	}
}

void AHealingPlant::SpawnHealingPlant()
{
	FVector myPos = GetActorLocation();
	float randPosX = FMath::RandRange(0, 10);
	float randPosY = FMath::RandRange(0, 10);
	FVector randPos = FVector(randPosX, randPosY, 10);
	SetActorLocation(myPos + randPos);
	if (myPos.Z >= 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnShakeHandle);
	}
}

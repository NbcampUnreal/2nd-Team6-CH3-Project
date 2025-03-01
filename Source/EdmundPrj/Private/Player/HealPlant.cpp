// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealPlant.h"
#include "Player\BaseCharacter.h"
#include "Components\SphereComponent.h"
AHealPlant::AHealPlant()
{
	CheckCharacterCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CheckCharacterCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealPlant::BeginOverlapCharacter);
	CheckCharacterCollision->OnComponentEndOverlap.AddDynamic(this, &AHealPlant::EndOverlapCharacter);
}

void AHealPlant::BeginOverlapCharacter(
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

void AHealPlant::EndOverlapCharacter(UPrimitiveComponent*
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


void AHealPlant::HealingCharacter()
{
	if (Character != nullptr)
	{
		//캐릭터 회복
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Meteor.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
#include "Components\SphereComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterLaunchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MonsterLaunchCollision"));
	MonsterLaunchCollision->SetupAttachment(Scene);

}

void AMeteor::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	if (!IsValid(Monster)) return;
	UGameplayStatics::ApplyDamage(
		Monster,
		DamageMultiplier,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
	HitToGround();
}

void AMeteor::HitToGround()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit To Ground!!"));
	TArray<AActor*> activators;
	MonsterLaunchCollision->GetOverlappingActors(activators);
	for (AActor* activator : activators)
	{
		if (activator && activator->ActorHasTag("Monster"))
		{
			if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(activator))
			{
				UGameplayStatics::ApplyDamage(
					Monster,
					DamageMultiplier,
					nullptr,
					this,
					UDamageType::StaticClass()
				);
				Monster->GetCharacterMovement()->Activate();
				UPrimitiveComponent* HitPrimitive = Cast<UPrimitiveComponent>(activator->GetRootComponent());

				if (HitPrimitive)
				{
					UE_LOG(LogTemp, Warning, TEXT("In HitPrimitive"))
					ACharacter* HitCharacter = Cast<ACharacter>(activator);
					if (HitCharacter)
					{
						HitCharacter->GetCharacterMovement()->StopMovementImmediately();
						FVector LaunchVector = HitCharacter->GetActorForwardVector() * MetoerHitPushStrength;
						float VerticalLaunchStrength = 300;;
						LaunchVector.Z = VerticalLaunchStrength;
						HitCharacter->LaunchCharacter(LaunchVector, false, false);
					}
				}

				
			}
		}
	}
	Deactivate();
}

void AMeteor::Deactivate()
{
	Super::Deactivate();
}

void AMeteor::SpawnTimerSkill()
{
	Super::SpawnTimerSkill();
}

void AMeteor::Tick(float deltaTime)
{
	FVector NewLocation = GetActorLocation() - FVector(0, 0, MoveSpeed);
	SetActorLocation(NewLocation);
}

void AMeteor::UpgradeSkill()
{
	DamageMultiplier += DamageMultiplierAmount;
}

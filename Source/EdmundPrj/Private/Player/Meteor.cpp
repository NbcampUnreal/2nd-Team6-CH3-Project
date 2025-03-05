// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Meteor.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
#include "Components\SphereComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
	MonsterLaunchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MonsterLaunchCollision"));
	MonsterLaunchCollision->SetupAttachment(Scene);

	GroundHitNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GroundHitNiagara"));
	GroundHitNiagara->SetupAttachment(RootComponent);
	GroundHitNiagara->bAutoActivate = false;

	FallingEffectNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FallingEffectNiagara"));
	FallingEffectNiagara->SetupAttachment(RootComponent);
	FallingEffectNiagara->bAutoActivate = false;
}


void AMeteor::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	if (!IsValid(Monster)) return;
	HitToGround();
}

TSet < TObjectPtr<ABaseMonster>> HitMonsterSet;
void AMeteor::HitToGround()
{
	TArray<AActor*> activators;
	MonsterLaunchCollision->GetOverlappingActors(activators);
	if (GroundHitNiagara)
	{
		GroundHitNiagara->Activate();
		GroundHitNiagara->SetWorldLocation(GetActorLocation());
		GetWorldTimerManager().SetTimer(ElplosionEffectDeactivateHandle,
			[this] {
				GroundHitNiagara->Deactivate();
			},
			5,
			false);
	}
	for (AActor* activator : activators)
	{
		if (activator && activator->ActorHasTag("Monster"))
		{
			if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(activator))
			{
				if (HitMonsterSet.Find(Monster))
				{
					continue;
				}
				else
				{
					HitMonsterSet.Add(Monster);
				}
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
}

void AMeteor::Deactivate()
{
	FallingEffectNiagara->Deactivate();
	HitMonsterSet.Empty();
	GetWorldTimerManager().ClearTimer(ElplosionEffectDeactivateHandle);
	Super::Deactivate();
}

void AMeteor::SpawnTimerSkill()
{
	Super::SpawnTimerSkill();
	FallingEffectNiagara->Activate();
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

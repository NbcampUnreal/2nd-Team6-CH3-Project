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

void AMeteor::HitToGround()
{
	TArray<AActor*> activators;
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		//GameState->PlayItemSound(AudioComponent, EItemType::HealKit)
	}
	MonsterLaunchCollision->GetOverlappingActors(activators);
	if (GroundHitNiagara)
	{
		UE_LOG(LogTemp, Warning, TEXT("asdfasdfasdf"));
		GroundHitNiagara->Activate();
		//GroundHitNiagara->SetWorldLocation(GetActorLocation());
		GetWorldTimerManager().SetTimer(ElplosionEffectDeactivateHandle,
			[this] {
				if (!IsValid(this)) return;
				GroundHitNiagara->Deactivate();
			},
			5,
			false);
	}
	for (AActor* activator : activators)
	{
		if (activator && activator->ActorHasTag("Monster") && !activator->ActorHasTag("Boss"))
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

void AMeteor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(ElplosionEffectDeactivateHandle);
}

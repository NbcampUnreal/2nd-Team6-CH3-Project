// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HealingPlant.h"
#include "Player\BaseCharacter.h"
#include "Components\SphereComponent.h"
AHealingPlant::AHealingPlant()
{
	CheckCharacterCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CheckCharacterCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CheckCharacterCollision->SetupAttachment(Scene);
}


void AHealingPlant::HealingCharacter()
{
	//파티클
	//TWeakObjectPtr<UParticleSystemComponent> TweakParticle;
	//if (ExplosionParticle)
	//{
	//	UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(
	//		GetWorld(),
	//		ExplosionParticle,
	//		GetActorLocation(),
	//		GetActorRotation(),
	//		false
	//	);
	//	TweakParticle = Particle;
	//}
	//사운드
	//if (ExplosionSound)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(
	//		GetWorld(),
	//		ExplosionSound,
	//		GetActorLocation()
	//	);
	//}
	TArray<AActor*> overlappingActors;
	CheckCharacterCollision->GetOverlappingActors(overlappingActors);
	for (AActor* activator : overlappingActors)
	{
		if (activator && activator->ActorHasTag("Player"))
		{
			Character = Cast<ABaseCharacter>(activator);
			Character->AmountHP(HealAmount);
		}
	}
	// 파티클 해제
	//if (TweakParticle.IsValid())
	//{
	//	FTimerHandle DestroyParticleTimerHandle;

	//	GetWorld()->GetTimerManager().SetTimer(
	//		DestroyParticleTimerHandle,
	//		[TweakParticle]()
	//		{
	//			if (TweakParticle.IsValid())
	//			{
	//				TweakParticle->DestroyComponent();
	//			}

	//		},
	//		2.0f,
	//		false
	//	);
	//}
}

void AHealingPlant::SpawnTimerSkill()
{
	GetWorldTimerManager().SetTimer(SpawnShakeHandle,
		[this] {
			FVector myPos = GetActorLocation();
			FVector randPos;;
			randPos.X = FMath::RandRange(myPos.X - 3, myPos.X + 3);
			randPos.Y = FMath::RandRange(myPos.Y - 3, myPos.Y + 3);
			randPos.Z = myPos.Z + 10;
			SetActorLocation(randPos);
			if (myPos.Z >= 0)
			{
				Super::SpawnTimerSkill();
				GetWorldTimerManager().ClearTimer(SpawnShakeHandle);
				GetWorldTimerManager().SetTimer(HealCycleHandle,
					this,
					&AHealingPlant::HealingCharacter,
					HealCycleTime,
					true);
			}
		},
		0.1f,
		true);
}

void AHealingPlant::Deactivate()
{
	GetWorldTimerManager().ClearTimer(HealCycleHandle);
	Super::Deactivate();
}

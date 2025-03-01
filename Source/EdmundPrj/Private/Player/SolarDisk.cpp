// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SolarDisk.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
ASolarDisk::ASolarDisk()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(Scene);
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(Scene);
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(Scene);
}

void ASolarDisk::HitToMonsterInCollision()
{
	TArray<AActor*> activators;
	EnemySearchCollision->GetOverlappingActors(activators);
	for(AActor* activator : activators)
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
			}
		}
	}
}

void ASolarDisk::SpawnTimerSkill()
{
	RotationElapsedTime = 0.0f; // 타이머 초기화
	GetWorldTimerManager().SetTimer(SpawnAnimHandle,
		[this] {
			FVector spherePos = SphereMesh->GetRelativeLocation();
			SphereMesh->SetRelativeLocation(spherePos + FVector(0, 0, 0.75f));
			if (spherePos.Z >= 310)
			{
				Super::SpawnTimerSkill();
				GetWorldTimerManager().ClearTimer(SpawnAnimHandle);
				GetWorldTimerManager().SetTimer(AttackCycleHandle,
					this,
					&ASolarDisk::HitToMonsterInCollision,
					AttackCycleSeconds,
					true);
				return;
			}
		},
		0.03f,
		true);
}

void ASolarDisk::Deactivate()
{
	Super::Deactivate();
	FVector spherePos = SphereMesh->GetRelativeLocation();
	spherePos.Z = 260;
	SphereMesh->SetRelativeLocation(spherePos);
}

void ASolarDisk::UpgradeSkill()
{
	DamageMultiplier += DamageMultiplierAmount;
}

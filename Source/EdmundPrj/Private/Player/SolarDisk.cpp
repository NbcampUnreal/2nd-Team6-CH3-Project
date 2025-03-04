// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SolarDisk.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
#include "Player\BaseCharacter.h"
ASolarDisk::ASolarDisk()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(Scene);
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(Scene);
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(Scene);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	TowerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
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
	TowerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spherePos.Z = 260;
	SphereMesh->SetRelativeLocation(spherePos);
}

void ASolarDisk::UpgradeSkill()
{
	DamageMultiplier += DamageMultiplierAmount;
}

void ASolarDisk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character)
	{
		// 목표 방향 계산
		FVector Direction = Character->GetActorLocation() - GetActorLocation();
		Direction.Z = 0; // Yaw 회전만 반영
		Direction.Normalize();

		// 목표 회전 계산
		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

		// 새로운 회전 적용
		SetActorRotation(NewRotation);
	}
}

void ASolarDisk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(SpawnAnimHandle);
}

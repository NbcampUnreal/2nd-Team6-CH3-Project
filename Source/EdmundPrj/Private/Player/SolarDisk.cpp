// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SolarDisk.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
#include "Player\BaseCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "System\EdmundGameState.h"
ASolarDisk::ASolarDisk()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(Scene);
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(Scene);
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(Scene);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagara"));
	HitNiagara->SetupAttachment(RootComponent);
	HitNiagara->bAutoActivate = false;
}

void ASolarDisk::HitToMonsterInCollision()
{
	TArray<AActor*> activators;
	HitNiagara->Deactivate();
	HitNiagara->Activate();
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		GameState->PlayItemSound(AudioComponent, EItemType::SolarDisk, ESoundType::Attack);
	}
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
			if (spherePos.Z >= 260)
			{
				Super::SpawnTimerSkill();
				GetWorldTimerManager().ClearTimer(SpawnAnimHandle);
				GetWorldTimerManager().SetTimer(AttackCycleHandle,
					this,
					&ASolarDisk::HitToMonsterInCollision,
					DamageCycleTime,
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
		FVector Direction = Character->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;
		Direction.Normalize();

		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

		SetActorRotation(NewRotation);
	}
}

void ASolarDisk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(SpawnAnimHandle);
}

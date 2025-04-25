// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/MonsterBulletPool.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "Monster/RangedMonsterBullet.h"
#include "Kismet/GameplayStatics.h"
#include "System/SpawnerHandle.h"


AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SpawnVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolumeComp"));
	SpawnVolumeComp->SetupAttachment(RootComp);

	SpawnerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMeshComp"));
	SpawnerMeshComp->SetupAttachment(RootComp);
}

void AMonsterSpawner::InitSpawner(AMonsterBulletPool* BulletPool, float NewSpawnTime, int32 NewSpawnCount, int32 NewLevelIndex)
{
	MonsterBulletPool = BulletPool;

	LevelIndex = NewLevelIndex;

	SpawnTime = NewSpawnTime;
	SpawnCount = NewSpawnCount;

	InitializeMonsterSpawnPool(SpawnCount);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnMonster, SpawnTime, true);

	SetBossMode(false);
}

void AMonsterSpawner::InitSpawner(AMonsterBulletPool* BulletPool, float NewSpawnTime, int32 NewSpawnCount)
{
	MonsterBulletPool = BulletPool;

	SpawnTime = NewSpawnTime;
	SpawnCount = NewSpawnCount;

	InitializeMonsterSpawnPool(SpawnCount);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnMonster, SpawnTime, true);

	SetBossMode(false);
}

void AMonsterSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearTimer();
}

void AMonsterSpawner::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void AMonsterSpawner::BossSpawn(ASpawnerHandle* NewSpawnerHandle, AMonsterBulletPool* BulletPool, int32 NewSpawnCount)
{
	SpawnerHandle = NewSpawnerHandle;
	MonsterBulletPool = BulletPool;
	InitializeMonsterSpawnPool(SpawnCount);
	BossModeSpawnCount = NewSpawnCount;

	for (int i = 0; i < BossModeSpawnCount; ++i)
	{
		SpawnMonster();
	}
	SetBossMode(true);

	for (ABaseMonster* Monster : SpawnedMonstersPool)
	{
		Monster->SetCanDropReward(false);
		Monster->SetChaseMode(true);
	}
}

void AMonsterSpawner::SetBossMode(bool NewMode)
{
	bBossMode = NewMode;
}

bool AMonsterSpawner::bCheckAllDead()
{
	return (BossModeSpawnCount <= DeadMonsterCount);
}

void AMonsterSpawner::AddDeadCount()
{
	if (!bBossMode) return;

	DeadMonsterCount++;

	if (bCheckAllDead())
	{
		if (IsValid(SpawnerHandle))
		{
			SpawnerHandle->IncreaseSpawnerClearCount();
		}
	}
}

void AMonsterSpawner::DestroySpawner()
{
	ClearTimer();

	for (ABaseMonster* Monster : SpawnedMonstersPool)
	{
		Monster->SetCanDropReward(false);
		Monster->MonsterDead();
	}

	Destroy();
}

void AMonsterSpawner::ApplyChaseMode()
{
	bIsDefenceMode = true;

	for (ABaseMonster* Monster : SpawnedMonstersPool)
	{
		Monster->SetChaseMode(true);
	}
}

ARangedMonsterBullet* AMonsterSpawner::GetBulletFromSpawner()
{
	return MonsterBulletPool->GetBulletFromPool();
}


void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}


ABaseMonster* AMonsterSpawner::GetMonsterFromPool()
{
	for (ABaseMonster* Monster : SpawnedMonstersPool)
	{
		if (!Monster)
		{
			return nullptr;
		}

 		if (Monster->IsHidden())
		{
			Monster->SetActorHiddenInGame(false);
			return Monster;
		}
	}

	return nullptr;
}

void AMonsterSpawner::InitializeMonsterSpawnPool(int32 PoolSize)
{
	FRotator SpawnRotation(GetActorRotation());
	FVector SpawnLocation(GetActorLocation());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnLocation.Z -= 2000;

		if (AllMonsters.Num() > 0)
		{
			int32 SelectedMonsterIndex = FMath::RandRange(0, AllMonsters.Num() - 1);
			MonsterClass = AllMonsters[SelectedMonsterIndex];

			if (IsValid(MonsterClass))
			{
				ABaseMonster* NewMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (NewMonster)
				{
					NewMonster->SetMonsterLevel(LevelIndex + 1);

					// 몬스터가 자신의 스포너를 참조하게 설정
					NewMonster->MonsterSpawner = this;
					NewMonster->SetActorHiddenInGame(true);
					SpawnedMonstersPool.Add(NewMonster);
				}
			}
		}
	}
}

void AMonsterSpawner::SpawnMonster()
{
	if (SpawnedMonstersPool.Num() > 0)
	{
		ABaseMonster* Monster = GetMonsterFromPool();
		if (Monster)
		{
			AAIController* AIController = Cast<AAIController>(Monster->GetController());
			if (AIController)
			{
				AIController->SetActorTickEnabled(true);
			}

			Monster->GetCharacterMovement()->Activate();
			Monster->SetMonsterLevel(LevelIndex + 1);
			Monster->SetMonsterStatsByLevel();
			Monster->Tags.Add(FName("Monster"));
			Monster->SetCanDropReward(true);
			Monster->SetIsDead(false);
			Monster->SetMonsterIsHit(false);
			Monster->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			Monster->GetCharacterMovement()->Velocity = FVector::ZeroVector;
			Monster->SetActorLocation(GetSpawnVolume());

			if (SpawnParticle)
			{
				UParticleSystemComponent* Particle = nullptr;

				FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);

				Particle = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					SpawnParticle,
					Monster->GetActorLocation(),
					Monster->GetActorRotation(),
					ParticleScale,
					false
				);
			}

			if (bIsDefenceMode)
			{
				Monster->SetChaseMode(true);
			}
		}
	}
}

FVector AMonsterSpawner::GetSpawnVolume()
{
	FVector BoxExtent = SpawnVolumeComp->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnVolumeComp->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}



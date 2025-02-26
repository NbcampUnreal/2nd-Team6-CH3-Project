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


// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SpawnVolumeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolumeComp"));
	SpawnVolumeComp->SetupAttachment(RootComp);

	SpawnerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMeshComp"));
	SpawnerMeshComp->SetupAttachment(RootComp);
}

void AMonsterSpawner::InitSpawner(AMonsterBulletPool* BulletPool, float NewSpawnTime, int32 NewSpawnCount)
{
	MonsterBulletPool = BulletPool;

	SpawnTime = NewSpawnTime;
	SpawnCount = NewSpawnCount;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnMonster, SpawnTime, true);
}

ARangedMonsterBullet* AMonsterSpawner::GetBulletFromSpawner()
{
	return MonsterBulletPool->GetBulletFromPool();
}


void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnMonster, SpawnTime, true);


	//InitializeMonsterSpawnPool(SpawnCount);

	//CurrentGameMode = GetWorld()->GetAuthGameMode();
	//if (CurrentGameMode)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Spawner가 CurrentGameMode를 가져옴"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("CurrentGameMode 없음"));
	//}
}


ABaseMonster* AMonsterSpawner::GetMonsterFromPool()
{
	for (ABaseMonster* Monster : SpawnedMonstersPool)
	{
		if (!Monster)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to retrieve a monster from the SpawnedMonsterpool."));
			return nullptr;
		}

 		if (Monster->IsHidden())
		{
			Monster->SetActorHiddenInGame(false);
			return Monster;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Full Spawn: %d"), SpawnCount);
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

			for (const TSubclassOf<ABaseMonster>& Monster : AllMonsters)
			{
				if (Monster == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("유효하지 않은 클래스가 발견되었습니다."));
				}
			}

			int32 SelectedMonsterIndex = FMath::RandRange(0, AllMonsters.Num() - 1);
			MonsterClass = AllMonsters[SelectedMonsterIndex];

			if (IsValid(MonsterClass))
			{
				ABaseMonster* NewMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterClass, SpawnLocation, SpawnRotation, SpawnParams);

				// 몬스터가 자신의 스포너를 참조하게 설정
				NewMonster->MonsterSpawner = this;

				if (!NewMonster)
				{
					UE_LOG(LogTemp, Error, TEXT("There is No Monster"))
				}

				if (NewMonster)
				{
					NewMonster->SetActorHiddenInGame(true);
					SpawnedMonstersPool.Add(NewMonster);
				}
			}

			else if (MonsterClass == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("MonsterClass is null"));
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
				//UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
				//BlackboardComp->SetValueAsBool(TEXT("HasLineOfSight"), false);
				//BlackboardComp->SetValueAsObject(TEXT("PlayerActor"), nullptr);

				AIController->SetActorTickEnabled(true);
			}
			Monster->SetIsDead(false);
			Monster->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			Monster->GetCharacterMovement()->Velocity = FVector::ZeroVector;
			Monster->SetActorLocation(GetSpawnVolume());
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



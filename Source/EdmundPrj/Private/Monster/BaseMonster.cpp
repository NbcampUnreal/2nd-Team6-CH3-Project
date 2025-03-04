// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseMonster.h"
#include "Player/BaseCharacter.h"
#include "Monster/HealingItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework//CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "AIController.h"
#include "Components/AudioComponent.h"
#include "Monster/MonsterSpawner.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "System/MissionHandle.h"

// 몬스터 스탯 설정은 여기서!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 여긴 베이스임 적용안됨
void ABaseMonster::SetMonsterStatsByLevel()
{
	MonsterHP = 100 + (MonsterLevel * 50);
	MonsterMaxHP = 100 + (MonsterLevel * 50);
	MonsterAttackDamage = 10.0f + (MonsterLevel * 5.0f);
	MonsterArmor = 5.0f + (MonsterLevel * 2.0f);
}


// Sets default values
ABaseMonster::ABaseMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);

	MonsterAttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
	MonsterAttackRange->SetupAttachment(RootComponent); // 공격 범위 콜리전

	USkeletalMeshComponent* MeshComp = GetMesh();

	MonsterOverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterOverHeadWidget"));
	MonsterOverHeadWidget->SetupAttachment(MeshComp);
	MonsterOverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MonsterOverHeadWidget->SetVisibility(false, true);

	GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeed;

	Tags.Add(FName("Monster"));

	MonsterType = EMonsterType::Melee;

	GameState = Cast<AEdmundGameState>(UGameplayStatics::GetGameState(GetWorld()));

}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	MonsterOverHeadWidget->SetVisibility(true, true);

	if (MonsterOverHeadWidget)
	{
		MonsterOverHeadWidgetObject = Cast<UAIInteractionWidget>(MonsterOverHeadWidget->GetUserWidgetObject());

		if (IsValid(MonsterOverHeadWidgetObject))
		{
			MonsterOverHeadWidgetObject->SetIsVisible(true);
			MonsterOverHeadWidgetObject->InitWidget();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MonsterOverHeadWidgetObject 없음"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterOverHeadWidget 없음"));
	}
}
void ABaseMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (bIsDead || bIsBondageMode) return 0;

	if (TakeDamageSound)
	{
		if (GameState)
		{
			GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Hit);
			//CurrentAudioComp->SetSound(TakeDamageSound);
			//CurrentAudioComp->Play();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamageSound가 없습니다."));
	}

	if (TakeDamageParticle)
	{
		SetChaseMode(true);
	

		UParticleSystemComponent* Particle = nullptr;

		FVector ParticleScale = FVector(2.0f, 2.0f, 2.0f);

		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			TakeDamageParticle,
			GetActorLocation(),
			GetActorRotation(),
			ParticleScale,
			false
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamageParticle이 없습니다."));
	}

	GetCharacterMovement()->Deactivate();

	float TakeDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamage = TakeDamageAmount * (1.0f - MonsterArmor / 100.0f);

	//UE_LOG(LogTemp, Warning, TEXT("원래 피해량: %f"), TakeDamageAmount);

	//UE_LOG(LogTemp, Warning, TEXT("감소된 피해량: %f"), ActualDamage);

	MonsterHP = FMath::Clamp(MonsterHP - ActualDamage, 0.0f, MonsterMaxHP);

	UpdateMonsterOverHeadWidget(ActualDamage);

	if (MonsterHP <= 0)
	{
		MonsterDead();
	}

	else
	{
		MonsterHit();
	}

	return ActualDamage;
}

// 죽는 애니메이션 재생 후 MonsterDestroy 호출
void ABaseMonster::MonsterDead()
{
	if (!bIsDead)
	{
		SetIsDead(true);

		if (bCanDropReward)
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				APawn* PlayerPawn = PlayerController->GetPawn();
				if (PlayerPawn)
				{
					ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(PlayerPawn);
					if (PlayerCharacter)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%f EXP얻음"), MonsterExpReward);
						PlayerCharacter->AddExp(MonsterExpReward);
					}
				}
			}
		}

		if (DeathAnimation)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				this->Tags.Remove(FName("Monster"));

				MonsterHP = 0;

				if (IsValid(MonsterOverHeadWidgetObject))
				{
					MonsterOverHeadWidgetObject->SetIsVisible(false);
				}

				GetWorld()->GetTimerManager().ClearTimer(HitAnimTimerHandle);
				GetWorld()->GetTimerManager().ClearTimer(AttackAnimTimerHandle);

				AnimInstance->Montage_Play(DeathAnimation);
				float AnimDuration = DeathAnimation->GetPlayLength();

				GetWorld()->GetTimerManager().SetTimer(DeadAnimTimerHandle, this, &ABaseMonster::MonsterDestroy, AnimDuration - 0.3f, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DeathAnimation이 없습니다."));
		}
	}
}

void ABaseMonster::SetIsDead(bool bNewIsDead)
{
	bIsDead = bNewIsDead;
}

void ABaseMonster::SetCanDropReward(bool NewState)
{
	bCanDropReward = NewState;
}

void ABaseMonster::SetMonsterLevel(int32 NewLevel)
{
	MonsterLevel = NewLevel;

	if (IsValid(MonsterOverHeadWidgetObject))
	{
		MonsterOverHeadWidgetObject->SetIsVisible(true);
	}
}

// DropReward 호출 후 Destroy
void ABaseMonster::MonsterDestroy()
{
	bIsDead = false;
	bIsHit = false;
	if (IsValid(MonsterOverHeadWidgetObject))
	{
		MonsterOverHeadWidgetObject->SetIsVisible(false);
	}

	if (bCanDropReward)
	{
		DropReward();
	}

	SetChaseMode(false);

	if (MonsterSpawner)
	{
		MonsterSpawner->AddDeadCount();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterSpawner가 없습니다."));
	}
	
	GetCharacterMovement()->Activate();

	SetActorHiddenInGame(true);

	MonsterHP = MonsterMaxHP;

	// 사망 시 바닥으로
	FVector GoToHell = GetActorLocation() + FVector(0, 0, -2000.0f);
	SetActorLocation(GoToHell);

	// 스폰될 때 까지 Tick 끄기
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->SetActorTickEnabled(false);
	}
}

void ABaseMonster::DropReward()
{

	float RandomValue = FMath::RandRange(0, 100);


	//인덱스 0번(힐)
	if (RandomValue <= MonsterHealKitProbability)
	{
		if (AllItems.Num() > 0)
		{
			if (IsValid(AllItems[0]))
			{
				ItemClass = AllItems[0];

				if (IsValid(ItemClass))
				{
					ABaseItem* NewMonster = GetWorld()->SpawnActor<ABaseItem>(ItemClass, GetActorLocation(), GetActorRotation());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("0번 아이템이 없습니다."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AllItems가 없습니다."));
		}
	}

	//인덱스 1번(골드)
	if (RandomValue <= MonsterGoldProbability)
	{
		if (AllItems.Num() > 0)
		{
			if (IsValid(AllItems[1]))
			{
				ItemClass = AllItems[1];

				if (IsValid(ItemClass))
				{
					ABaseItem* NewMonster = GetWorld()->SpawnActor<ABaseItem>(ItemClass, GetActorLocation(), GetActorRotation());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("1번 아이템이 없습니다."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AllItems가 없습니다."));
		}
	}

	//인덱스 2번(왕골드)
	if (RandomValue <= MonsterGoldProbability / 3)
	{
		if (AllItems.Num() > 0)
		{
			if (IsValid(AllItems[2]))
			{
				ItemClass = AllItems[2];

				if (IsValid(ItemClass))
				{
					ABaseItem* NewMonster = GetWorld()->SpawnActor<ABaseItem>(ItemClass, GetActorLocation(), GetActorRotation());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("2번 아이템이 없습니다."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AllItems가 없습니다."));
		}
	}
}

void ABaseMonster::MonsterHit()
{
	if (bIsDead) return;

	if (HitAnimation)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			bIsHit = true;

			GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 1));
			GetWorld()->GetTimerManager().ClearTimer(HitAnimTimerHandle);

			AnimInstance->Montage_Play(HitAnimation);

			float AnimDuration = HitAnimation->GetPlayLength();

			GetWorld()->GetTimerManager().SetTimer(HitAnimTimerHandle, this, &ABaseMonster::MonsterHitEnd, AnimDuration, false);
		}
	}
}

void ABaseMonster::MonsterHitEnd()
{
	bIsHit = false;
	if (!bIsFrozen)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, HitAnimation);
		GetCharacterMovement()->Activate();
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ABaseMonster::MonsterAttack()
{

	if (bIsDead || bIsHit) return;

	if (AttackAnimation)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 0, 1));

			AnimInstance->Montage_Play(AttackAnimation);

			float AnimDuration = AttackAnimation->GetPlayLength();

			GetWorld()->GetTimerManager().SetTimer(AttackAnimTimerHandle, this, &ABaseMonster::MonsterAttackEnd, AnimDuration, false);
		}
	}
}

void ABaseMonster::SetChaseMode(bool Mode)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn();

			AIController->GetBlackboardComponent()->SetValueAsBool(FName("HasLineOfSight"), Mode);
			AIController->GetBlackboardComponent()->SetValueAsObject(FName("PlayerActor"), PlayerPawn);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ChaseMode 실행중: PlayerController가 없습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ChaseMode 실행중: AIController가 없습니다."));
	}
}


void ABaseMonster::MonsterAttackEnd()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.3f, AttackAnimation);

	//주석 처리를 했는데 왜 움직일까?
	//GetCharacterMovement()->Activate();
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ABaseMonster::MonsterAttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("몬스터에 MonsterAttackCheck 함수가 없습니다."));
}

float ABaseMonster::GetMonsterAttackDamage()
{
	return MonsterAttackDamage;
}

void ABaseMonster::UpdateMonsterOverHeadWidget(float Damage)
{
	if (!MonsterOverHeadWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("위젯 X"));
		return;
	}
	if (IsValid(MonsterOverHeadWidgetObject))
	{
		MonsterOverHeadWidgetObject->SetIsVisible(true);
	}

	//UUserWidget* MonsterOverHeadWidgetInstance = MonsterOverHeadWidget->GetUserWidgetObject();

	//if (!MonsterOverHeadWidgetInstance) return;

	//MonsterOverHeadWidget->SetVisibility(true, true);
	//GetWorld()->GetTimerManager().ClearTimer(OverHeadUITimerHandle);

	//if (UProgressBar* HPBar = Cast<UProgressBar>(MonsterOverHeadWidgetInstance->GetWidgetFromName(TEXT("HealthBar"))))
	//{
	//	float HealthPercent = MonsterHP / MonsterMaxHP;
	//	HPBar->SetPercent(HealthPercent);
	//}

	//GetWorld()->GetTimerManager().SetTimer(OverHeadUITimerHandle, this, &ABaseMonster::UpdateMonsterOverHeadWidgetEnd, 1.0f, false);


	if (IsValid(MonsterOverHeadWidgetObject))
	{
		MonsterOverHeadWidgetObject->ApplyHitEvent(MonsterMaxHP, MonsterHP, Damage);
	}
}

void ABaseMonster::UpdateMonsterOverHeadWidgetEnd()
{
	MonsterOverHeadWidget->SetVisibility(false, true);
}

void ABaseMonster::PlayParticle()
{
}

void ABaseMonster::PlaySound()
{
}

void ABaseMonster::SetInitialSpawn()
{
	bIsInitialSpawn = false;
}

void ABaseMonster::UpdateChaseSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MonsterCurrentSpeed * MonsterChaseSpeedMultifly;
}

void ABaseMonster::UpdatePatrolSpeed()
{
	//이 함수는 쓰이지 않음
	GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeed;
}

void ABaseMonster::ChangeCurrentSpeed(float ChangeSpeed)
{
	MonsterCurrentSpeed = ChangeSpeed;
}

float ABaseMonster::GetCurrentSpeed()
{
	return MonsterCurrentSpeed;
}

void ABaseMonster::UpdateState(EMonsterState NewMonsterState)
{
	MonsterState = NewMonsterState;
}

void ABaseMonster::FreezeMonster(float FreezeDuration)
{
	if (bIsDead || bIsFrozen) return; // 이미 죽었거나 얼어 있으면 실행 X

	bIsFrozen = true; // 상태 변경

	// 이동 멈춤
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	// 현재 애니메이션 상태 유지 후 정지
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f); // 즉시 중단 (필요 시)
		GetMesh()->bPauseAnims = true; // 애니메이션 정지
	}

	// 일정 시간 후 해제
	GetWorldTimerManager().SetTimer(FreezeTimerHandle, this, &ABaseMonster::UnfreezeMonster, FreezeDuration, false);
}

void ABaseMonster::UnfreezeMonster()
{
	bIsFrozen = false; // 상태 해제

	// 이동 복구
	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, HitAnimation);
	GetCharacterMovement()->Activate();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 애니메이션 재개
	GetMesh()->bPauseAnims = false;
}
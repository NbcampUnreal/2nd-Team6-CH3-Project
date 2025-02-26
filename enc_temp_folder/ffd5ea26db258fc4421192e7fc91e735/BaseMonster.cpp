// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseMonster.h"
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
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GetCharacterMovement()->Deactivate();

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MonsterHP = FMath::Clamp(MonsterHP - ActualDamage, 0.0f, MonsterMaxHP);

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
		if (DeathAnimation)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				MonsterHP = 0;

				UpdateMonsterOverHeadWidget();

				GetWorld()->GetTimerManager().ClearTimer(HitAnimTimerHandle);
				GetWorld()->GetTimerManager().ClearTimer(AttackAnimTimerHandle);

				AnimInstance->Montage_Play(DeathAnimation);
				float AnimDuration = DeathAnimation->GetPlayLength();

				GetWorld()->GetTimerManager().SetTimer(DeadAnimTimerHandle, this, &ABaseMonster::MonsterDestroy, AnimDuration - 0.3f, false);
			}
		}
	}
}

void ABaseMonster::SetIsDead(bool bNewIsDead)
{
	bIsDead = bNewIsDead;
}

// DropReward 호출 후 Destroy
void ABaseMonster::MonsterDestroy()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, DeathAnimation);

	DropReward();

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

	if (RandomValue <= MonsterGoldProbability)
	{
		//바닥에 Gold가 떨어짐
	}

	if (RandomValue <= MonsterHealKitProbability)
	{
		//바닥에 Healkit이 떨어짐
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
			if (TakeDamageParticle)
			{
				UpdateMonsterOverHeadWidget();

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

			if (TakeDamageSound)
			{
				CurrentAudioComp->SetSound(TakeDamageSound);
				CurrentAudioComp->Play();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TakeDamageSound가 없습니다."));
			}

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

	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, HitAnimation);
	GetCharacterMovement()->Activate();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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

void ABaseMonster::UpdateMonsterOverHeadWidget()
{
	if (!MonsterOverHeadWidget) return;

	UUserWidget* MonsterOverHeadWidgetInstance = MonsterOverHeadWidget->GetUserWidgetObject();

	if (!MonsterOverHeadWidgetInstance) return;

	MonsterOverHeadWidget->SetVisibility(true, true);
	GetWorld()->GetTimerManager().ClearTimer(OverHeadUITimerHandle);

	if (UProgressBar* HPBar = Cast<UProgressBar>(MonsterOverHeadWidgetInstance->GetWidgetFromName(TEXT("HealthBar"))))
	{
		float HealthPercent = MonsterHP / MonsterMaxHP;
		HPBar->SetPercent(HealthPercent);
	}

	GetWorld()->GetTimerManager().SetTimer(OverHeadUITimerHandle, this, &ABaseMonster::UpdateMonsterOverHeadWidgetEnd, 1.0f, false);

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
	GetCharacterMovement()->MaxWalkSpeed = MonsterChaseSpeed;
}

void ABaseMonster::UpdatePatrolSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeed;
}

void ABaseMonster::UpdateState(EMonsterState NewMonsterState)
{
	MonsterState = NewMonsterState;
}


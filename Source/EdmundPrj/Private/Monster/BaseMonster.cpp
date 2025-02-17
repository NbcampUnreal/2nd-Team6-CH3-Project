// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseMonster.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MonsterAttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
	MonsterAttackRange->SetupAttachment(RootComponent); // 공격 범위 콜리전

	USkeletalMeshComponent* MeshComp = GetMesh();

	MonsterOverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterOverHeadWidget"));
	MonsterOverHeadWidget->SetupAttachment(MeshComp);
	MonsterOverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MonsterHP = FMath::Clamp(MonsterHP - ActualDamage, 0.0f, MonsterMaxHP);

	// OverHeadWidget 업데이트
	UpdateMonsterOverHeadWidget();

	if (MonsterHP <= 0)
	{
		MonsterDead();
	}

	return ActualDamage;
}

// 죽는 애니메이션 재생 후 MonsterDestroy 호출
void ABaseMonster::MonsterDead()
{
	if (DeathAnimation)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DeathAnimation);

			float AnimDuration = DeathAnimation->GetPlayLength();

			GetWorld()->GetTimerManager().SetTimer(DeadAnimTimerHandle, this, &ABaseMonster::MonsterDestroy, AnimDuration, false);
		}
	}
}

// DropReward 호출 후 Destroy
void ABaseMonster::MonsterDestroy()
{
	DropReward();
	Destroy();
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

void ABaseMonster::UpdateMonsterOverHeadWidget()
{
	if (!MonsterOverHeadWidget) return;

	UUserWidget* MonsterOverHeadWidgetInstance = MonsterOverHeadWidget->GetUserWidgetObject();

	if (!MonsterOverHeadWidgetInstance) return;

	if (UProgressBar* HPBar = Cast<UProgressBar>(MonsterOverHeadWidgetInstance->GetWidgetFromName(TEXT("HealthBar"))))
	{
		float HealthPercent = MonsterHP / MonsterMaxHP;
		HPBar->SetPercent(HealthPercent);
	}
}


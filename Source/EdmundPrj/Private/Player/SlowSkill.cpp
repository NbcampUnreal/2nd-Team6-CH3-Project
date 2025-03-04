// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlowSkill.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"

void ASlowSkill::HitToMonster(TObjectPtr<ABaseMonster> monster)
{
	if (!IsValid(monster)) return;
	float CurrentSpeed = monster->GetCurrentSpeed();
	monster->ChangeCurrentSpeed(CurrentSpeed - SlowApplicationRate);
	UGameplayStatics::ApplyDamage(
		monster,
		DamageMultiplier,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
	GetWorldTimerManager().SetTimer(SlowHandle,
		[CurrentSpeed, monster] {
			if (!IsValid(monster)) return;
			monster->ChangeCurrentSpeed(CurrentSpeed);
		},
		SlowTime,
		false
		);
}

void ASlowSkill::UpgradeSkill()
{
	SlowApplicationRate += AmountSlowApplicationRate;
	DamageMultiplier += DamageMultiplierAmount;
}

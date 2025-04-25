#include "Player/PlayerCharacterAurora.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

APlayerCharacterAurora::APlayerCharacterAurora()
{
	PrimaryActorTick.bCanEverTick = false;

	ComboCount = 0;
	ResetDelay = 1.0f;
	AttackRangeRadius = 150.0f;
	ComboTimeDuration = 1.2f;
	AttackForwardOffset = 10.0f;
}

void APlayerCharacterAurora::BeginPlay()
{
	Super::BeginPlay();

	ComboTimeDuration = ComboTimeDuration - AttackDelay;
}

void APlayerCharacterAurora::Attack(const FInputActionValue& value)
{
	if (IsAttack)
	{
		return;
	}

	check(AttackMontages[ComboCount] && CurrentGameState);

	IsAttack = true;

	Super::Attack(value);

	PlayAnimMontage(AttackMontages[ComboCount], ComboTimeDuration);

	switch (ComboCount)
	{
		case 0:
			CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack);
			break;

		case 1:
			CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack2);
			break;

		case 2:
			CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack3);
			break;

		case 3:
			CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack4);
			break;

		default:
			break;
	}

	// 콤보 공격 딜레이
	GetWorld()->GetTimerManager().SetTimer(
		ComboResetTimerHandle,
		this,
		&ThisClass::ResetCombo,
		ResetDelay,
		false
	);
}

void APlayerCharacterAurora::AttackTrace()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FRotator ControlRotation = PlayerController->GetControlRotation();
	FVector ForwardVector = ControlRotation.Vector();

	FVector Start = GetActorLocation() + (ForwardVector * (AttackRangeRadius + AttackForwardOffset)); // 공격 시작 위치
	FVector End = Start + (ForwardVector * (AttackRangeRadius + AttackForwardOffset)); // 공격 끝 위치

	TArray<FHitResult> HitResults;

	// 트레이스 수행
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자신은 무시하도록 설정

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // 시작 위치
		End,                 // 끝 위치
		FQuat::Identity,     // 회전값 (회전 없이)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // 충돌 채널
		FCollisionShape::MakeSphere(AttackRangeRadius), // 범위 설정 (구체 모양)
		QueryParams
	);

	// 데미지를 입힌 액터를 추적할 Set (중복 방지)
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		return;
	}

	// 여러 충돌 객체가 있다면
	for (const FHitResult& Hit : HitResults)
	{
		// 충돌한 객체가 있다면
		AActor* HitActor = Hit.GetActor();

		if (IsValid(HitActor) && !DamagedActors.Contains(HitActor) && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
		{
			UGameplayStatics::ApplyDamage(
				HitActor,
				GetAttackDamage(),
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}

		// 데미지를 입힌 액터를 Set에 추가
		DamagedActors.Add(HitActor);
	}
}

void APlayerCharacterAurora::NextCombo()
{
	IsAttack = false;

	ComboCount = ++ComboCount % AttackMontages.Num();
}

void APlayerCharacterAurora::ResetCombo()
{
	ComboCount = 0;
}

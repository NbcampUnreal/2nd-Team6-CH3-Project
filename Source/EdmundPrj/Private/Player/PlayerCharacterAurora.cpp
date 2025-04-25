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

	// �޺� ���� ������
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

	FVector Start = GetActorLocation() + (ForwardVector * (AttackRangeRadius + AttackForwardOffset)); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * (AttackRangeRadius + AttackForwardOffset)); // ���� �� ��ġ

	TArray<FHitResult> HitResults;

	// Ʈ���̽� ����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڽ��� �����ϵ��� ����

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // ���� ��ġ
		End,                 // �� ��ġ
		FQuat::Identity,     // ȸ���� (ȸ�� ����)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // �浹 ä��
		FCollisionShape::MakeSphere(AttackRangeRadius), // ���� ���� (��ü ���)
		QueryParams
	);

	// �������� ���� ���͸� ������ Set (�ߺ� ����)
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		return;
	}

	// ���� �浹 ��ü�� �ִٸ�
	for (const FHitResult& Hit : HitResults)
	{
		// �浹�� ��ü�� �ִٸ�
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

		// �������� ���� ���͸� Set�� �߰�
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

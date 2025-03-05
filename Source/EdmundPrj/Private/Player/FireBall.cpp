#include "Player/FireBall.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"
#include "System/EdmundGameState.h"
#include "System/EnumSet.h"

AFireBall::AFireBall() : Super()
{
	AttackRadiusMultifler = 6.0f;

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);
}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
}

void AFireBall::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet")) || bIsHidden)
	{
		return;
	}

	EndBulletLife();
}

void AFireBall::EndBulletLife()
{
	Super::EndBulletLife();

	// ���� ����Ʈ ���
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f) * AttackRadiusMultifler);
	}

	AGameStateBase* GameStateBase = GetWorld()->GetGameState();

	AEdmundGameState* CurrentGameState = Cast<AEdmundGameState>(GameStateBase);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Weapon);
	}

	SetBulletHidden(true);

	FVector Start = GetActorLocation(); // ���� ���� ��ġ
	FVector End = Start; // ���� �� ��ġ

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
		FCollisionShape::MakeSphere(100.0f * AttackRadiusMultifler), // ���� ���� (��ü ���)
		QueryParams
	);

	// �������� ���� ���͸� ������ Set (�ߺ� ����)
	// Set�� ������ ���������ѹ��� ������ ������ �޴� ���� �߻�
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		bool IsBossAttack = false;

		// ���� �浹 ��ü�� �ִٸ�
		for (const FHitResult& Hit : HitResults)
		{
			// �浹�� ��ü�� �ִٸ�
			AActor* HitActor = Hit.GetActor();

			if (!DamagedActors.Contains(HitActor) && HitActor && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster") || HitActor->ActorHasTag("Area") || HitActor->ActorHasTag("NPC")))
			{
				if (!IsBossAttack && HitActor->ActorHasTag("Boss"))
				{
					if (IsBossAttack)
					{
						continue;
					}

					IsBossAttack = true;
				}

				float Damage = 30.0f;

				// ���� �÷��̾� ĳ���ͷκ��� ������ ���� ��� �ڵ�
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);  // 0�� ù ��° �÷��̾�
				if (PlayerController)
				{
					APawn* ControlledPawn = PlayerController->GetPawn();
					if (ControlledPawn)
					{
						ABaseCharacter* Player = Cast<ABaseCharacter>(ControlledPawn);
						if (IsValid(Player))
						{
							Damage = Player->GetAttackDamage();
						}
					}
				}

				UGameplayStatics::ApplyDamage(
					HitActor,
					Damage,
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}

			// �������� ���� ���͸� Set�� �߰�
			DamagedActors.Add(HitActor);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ParticleCreateDelayTimerHandle);
}

void AFireBall::SetBulletHidden(bool IsHidden)
{
	bIsHidden = IsHidden;

	// 3�� �ڱ��� ������ �ȵ� ��� Ǯ��
	if (!IsHidden)
	{
		GetWorld()->GetTimerManager().SetTimer(
			BulletLifeTimerHandle,
			this,
			&ABullet::EndBulletLife,
			3.0f,
			false
		);

		GetWorld()->GetTimerManager().SetTimer(
			ParticleCreateDelayTimerHandle,
			this,
			&AFireBall::CreateParticle,
			0.03f,
			true
		);
	}

	SetActorHiddenInGame(bIsHidden);  // �Ѿ� ���� ó��
}

void AFireBall::CreateParticle()
{
	if (bIsHidden)
	{
		return;
	}

	// ��ƼŬ ����
	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireBallParticle, GetActorLocation(), GetActorRotation(), true);
}

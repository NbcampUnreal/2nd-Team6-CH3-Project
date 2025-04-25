#include "Player/ChargingBullet.h"
#include "Player/PlayerCharacterWraith.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

AChargingBullet::AChargingBullet() : Super()
{
	// ��¡�� ���� �ٲ�
	BulletDamage = 30;
	IsBossAttack = false;

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);
}

void AChargingBullet::SetBulletScale()
{
	// ���� �� ���� ������
	IsBossAttack = false;

	ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(PlayerCharacter))
	{
		APlayerCharacterWraith* Player = Cast<APlayerCharacterWraith>(PlayerCharacter);

		if (IsValid(Player))
		{
			SetActorRelativeScale3D(Player->GetBulletScale());
		}
	}
}

void AChargingBullet::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsHidden || !IsValid(OtherActor) || (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Area") || OtherActor->ActorHasTag("NPC")))
	{
		return;
	}

	// ���ʹ� ����
	if ((OtherActor->ActorHasTag("Monster")))
	{
		SetBulletHidden(true);
	}

	if (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem"))
	{
		if (OtherActor->ActorHasTag("Boss"))
		{
			if (IsBossAttack)
			{
				return;
			}

			IsBossAttack = true;
		}

		float Damage = BulletDamage;

		// ���� ������ ����
		float RandomRange = 0.2f;

		float MinDamage = (1.0f - RandomRange) * Damage;
		float MaxDamage = (1.0f + RandomRange) * Damage;

		Damage = FMath::RandRange(MinDamage, MaxDamage);

		// ������ ����
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
	}

	else
	{
		SetBulletHidden(true);
	}

	// �浹 �߻� �� ó��
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation());
	}

	AEdmundGameState* CurrentGameState = GetWorld()->GetGameState<AEdmundGameState>();

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Weapon);
	}
}

void AChargingBullet::SetBulletDamage(float NewDamage)
{
	BulletDamage = NewDamage;
}

void AChargingBullet::BeginPlay()
{
	Super::BeginPlay();
}

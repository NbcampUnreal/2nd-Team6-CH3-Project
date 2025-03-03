#include "Player/ChargingBullet.h"
#include "Player/PlayerCharacterWraith.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

AChargingBullet::AChargingBullet() : Super()
{
	// ��¡�� ���� �ٲ�
	BulletDamage = 30;

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);
}

void AChargingBullet::SetBulletScale()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(PlayerCharacter))
	{
		APlayerCharacterWraith* Player = Cast<APlayerCharacterWraith>(PlayerCharacter);

		if (IsValid(Player) && IsValid(Player->BulletMesh))
		{
			SetActorRelativeScale3D(Player->BulletMesh->GetRelativeScale3D());
		}
	}
}

void AChargingBullet::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsHidden || (OtherActor && (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet"))))
	{
		return;
	}

	// �浹 �߻� �� ó��
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation());
	}

	AGameStateBase* GameStateBase = GetWorld()->GetGameState();

	AEdmundGameState* CurrentGameState = Cast<AEdmundGameState>(GameStateBase);
	
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Weapon);
	}

	// ���ʹ� ����
	if (OtherActor && !(OtherActor->ActorHasTag("Monster")))
	{
		SetBulletHidden(true);
	}

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")))
	{
		float Damage = 30.0f;

		Damage = BulletDamage;

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
}

void AChargingBullet::SetBulletDamage(float NewDamage)
{
	BulletDamage = NewDamage;
}

void AChargingBullet::BeginPlay()
{
	Super::BeginPlay();
}

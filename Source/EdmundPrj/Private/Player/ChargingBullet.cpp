#include "Player/ChargingBullet.h"
#include "Player/PlayerCharacterWraith.h"
#include "Kismet/GameplayStatics.h"

AChargingBullet::AChargingBullet() : Super()
{
	// ��¡�� ���� �ٲ�
	BulletDamage = 30;
}

void AChargingBullet::SetBulletHidden(bool IsHidden)
{
	Super::SetBulletHidden(IsHidden);

	if (IsHidden)
	{
		return;
	}

	if (!IsValid(Player))
	{
		return;
	}

	if (!IsValid(Player->BulletMesh))
	{
		return;
	}

	SetActorRelativeScale3D(Player->BulletMesh->GetRelativeScale3D());
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

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")))
	{
		float Damage = 30.0f;

		Damage = BulletDamage;

		// ������ ����
		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, nullptr, this, UDamageType::StaticClass());
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

	ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerCharacter)
	{
		Player = Cast<APlayerCharacterWraith>(PlayerCharacter);
	}
}

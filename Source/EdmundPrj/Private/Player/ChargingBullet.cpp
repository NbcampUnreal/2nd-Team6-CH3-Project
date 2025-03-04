#include "Player/ChargingBullet.h"
#include "Player/PlayerCharacterWraith.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

AChargingBullet::AChargingBullet() : Super()
{
	// 차징에 따라 바뀜
	BulletDamage = 30;
	IsBossAttack = false;

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);
}

void AChargingBullet::SetBulletScale()
{
	// 보스 한 번만 때리게
	IsBossAttack = false;

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
	if (bIsHidden || (OtherActor && (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Area") || OtherActor->ActorHasTag("NPC"))))
	{
		return;
	}

	AGameStateBase* GameStateBase = GetWorld()->GetGameState();

	AEdmundGameState* CurrentGameState = Cast<AEdmundGameState>(GameStateBase);

	// 몬스터는 관통
	if (OtherActor && !(OtherActor->ActorHasTag("Monster")))
	{
		SetBulletHidden(true);
	}

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")))
	{
		if (OtherActor->ActorHasTag("Boss"))
		{
			if (IsBossAttack)
			{
				return;
			}

			IsBossAttack = true;
		}

		float Damage = 30.0f;

		Damage = BulletDamage;

		// 랜덤 데미지 적용
		float RandomRange = 0.2f;

		float MinDamage = (1.0f - RandomRange) * Damage;
		float MaxDamage = (1.0f + RandomRange) * Damage;

		Damage = FMath::RandRange(MinDamage, MaxDamage);

		// 데미지 적용
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
	}

	else
	{
		SetBulletHidden(true);
	}

	// 충돌 발생 시 처리
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation());
	}

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

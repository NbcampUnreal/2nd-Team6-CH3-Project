#include "Player/FireBall.h"
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
	if (bIsHidden || !IsValid(OtherActor) || (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet") || OtherActor->ActorHasTag("Area") || OtherActor->ActorHasTag("NPC")))
	{
		return;
	}

	EndBulletLife();
}

void AFireBall::EndBulletLife()
{
	Super::EndBulletLife();

	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f) * AttackRadiusMultifler);
	}

	AEdmundGameState* CurrentGameState = GetWorld()->GetGameState<AEdmundGameState>();

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Weapon);
	}

	SetBulletHidden(true);

	FVector Start = GetActorLocation();
	FVector End = Start;

	TArray<FHitResult> HitResults;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_OverlapAll_Deprecated,
		FCollisionShape::MakeSphere(100.0f * AttackRadiusMultifler),
		QueryParams
	);

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ParticleCreateDelayTimerHandle);

	TSet<AActor*> DamagedActors;

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!DamagedActors.Contains(HitActor) && HitActor && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
		{
			float Damage = 30.0f;

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

			if (IsValid(PlayerController))
			{
				ABaseCharacter* Player = PlayerController->GetPawn<ABaseCharacter>();

				if (IsValid(Player))
				{
					Damage = Player->GetAttackDamage();
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

		DamagedActors.Add(HitActor);
	}
}

void AFireBall::SetBulletHidden(bool IsHidden)
{
	Super::SetBulletHidden(IsHidden);

	bIsHidden = IsHidden;

	if (!IsHidden)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ParticleCreateDelayTimerHandle,
			this,
			&ThisClass::CreateParticle,
			0.03f,
			true
		);
	}

	SetActorHiddenInGame(bIsHidden);
}

void AFireBall::CreateParticle()
{
	if (bIsHidden)
	{
		return;
	}

	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireBallParticle, GetActorLocation(), GetActorRotation(), true);
}

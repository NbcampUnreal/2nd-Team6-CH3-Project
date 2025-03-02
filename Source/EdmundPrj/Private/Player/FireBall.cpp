#include "Player/FireBall.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"

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

	// 폭발 이펙트 재생
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f) * AttackRadiusMultifler);
	}

	// 폭발 사운드
	if (FireSound)
	{
		CurrentAudioComp->SetSound(FireSound);
		CurrentAudioComp->Play();
	}

	SetBulletHidden(true);

	FVector Start = GetActorLocation(); // 공격 시작 위치
	FVector End = Start; // 공격 끝 위치

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
		FCollisionShape::MakeSphere(100.0f * AttackRadiusMultifler), // 범위 설정 (구체 모양)
		QueryParams
	);

	// 구체 보이게 하기
	if (GEngine)
	{
		DrawDebugSphere(
			GetWorld(),
			Start,
			100.0f * AttackRadiusMultifler,
			12,
			FColor::Red,        // 색상
			false,              // 지속성 (게임 중 계속 표시할지 여부)
			1.0f                // 지속 시간
		);
	}

	// 데미지를 입힌 액터를 추적할 Set (중복 방지)
	// Set이 없으면 근접공격한번에 여러번 데미지 받는 현상 발생
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		// 여러 충돌 객체가 있다면
		for (const FHitResult& Hit : HitResults)
		{
			// 충돌한 객체가 있다면
			AActor* HitActor = Hit.GetActor();

			if (!DamagedActors.Contains(HitActor) && HitActor && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					30.0f,
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}

			// 데미지를 입힌 액터를 Set에 추가
			DamagedActors.Add(HitActor);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ParticleCreateDelayTimerHandle);
}

void AFireBall::SetBulletHidden(bool IsHidden)
{
	bIsHidden = IsHidden;

	// 3초 뒤까지 오버랩 안될 경우 풀링
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

	SetActorHiddenInGame(bIsHidden);  // 총알 숨김 처리
}

void AFireBall::CreateParticle()
{
	if (bIsHidden)
	{
		return;
	}

	// 파티클 생성
	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireBallParticle, GetActorLocation(), GetActorRotation(), true);
}

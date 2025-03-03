#include "Player/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(StaticMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	BulletSpeed = 10000.0f;
	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	BulletDuraion = 3.0f;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileOverlap);

	BulletLandParticle = nullptr;

	bIsHidden = true;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	SetBulletHidden(true);
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentProjectileLocation = GetActorLocation();

	// Sweep 충돌 검사
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // 총알 자체는 무시

	// SweepMultiByChannel을 사용하여 충돌을 검사
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,    // 충돌 결과
		PrevProjectileLocation,        // 시작 위치
		CurrentProjectileLocation,          // 끝 위치 (이동 후)
		FQuat::Identity,  // 회전값 (회전 없이 이동)
		ECC_Visibility,   // 충돌 채널
		FCollisionShape::MakeSphere(32.0f)  // 범위 설정 (구체 형태)
	);

	if (bHit)
	{
		// 충돌한 객체가 있다면
		OnProjectileOverlap(nullptr, HitResult.GetActor(), nullptr, 0, true, HitResult);
	}
}

void ABaseProjectile::SetBulletHidden(bool IsHidden)
{
	bIsHidden = IsHidden;
	// 3초 뒤까지 오버랩 안될 경우 풀링
	if (!IsHidden)
	{
		PrevProjectileLocation = GetActorLocation();
		PrimaryActorTick.bCanEverTick = true;

		GetWorld()->GetTimerManager().SetTimer(
			BulletLifeTimerHandle,
			this,
			&ABaseProjectile::EndBulletLife,
			BulletDuraion,
			false
		);
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
	}
	SetActorHiddenInGame(bIsHidden);  // 총알 숨김 처리
}

void ABaseProjectile::EndBulletLife()
{
	SetBulletHidden(true);
}

void ABaseProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsHidden || (OtherActor && (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet"))))
	{
		return;
	}

	// 충돌 발생 시 처리
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation());
	}

	SetBulletHidden(true);

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")))
	{
		float Damage = 30.0f;

		// 현재 플레이어 캐릭터로부터 데미지 값을 얻는 코드
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);  // 0은 첫 번째 플레이어
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

		// 데미지 적용
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
}
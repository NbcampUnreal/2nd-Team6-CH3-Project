#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Boss/Boss.h"
#include "Engine/World.h"

TArray<ABoss_Attack1_Bullet*> ABoss_Attack1_Bullet::BulletPool;

ABoss_Attack1_Bullet::ABoss_Attack1_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 생성 및 설정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->SetCollisionProfileName(TEXT("IgnoreAll"));
	// Pawn 채널은 Overlap 처리하여 물리 밀림 방지
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// CCD 활성화 (빠른 이동에도 충돌 감지)
	CollisionComp->BodyInstance.bUseCCD = true;
	// Hit와 Overlap 이벤트 바인딩
	CollisionComp->OnComponentHit.AddDynamic(this, &ABoss_Attack1_Bullet::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Attack1_Bullet::OnOverlapBegin);
	RootComponent = CollisionComp;

	// 총알 메쉬 컴포넌트 생성 및 설정 (충돌은 CollisionComp가 처리)
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 폭발 파티클 이펙트 컴포넌트 생성 및 설정 (자동 재생 비활성)
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	ExplosionEffect->SetupAttachment(RootComponent);
	ExplosionEffect->bAutoActivate = false;

	Attack1Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Attack1Niagara"));
	Attack1Niagara->SetupAttachment(RootComponent);
	Attack1Niagara->bAutoActivate = false;


	// 초기 상태: 탄환은 숨기고 충돌 비활성
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}


void ABoss_Attack1_Bullet::BeginPlay()
{
	Super::BeginPlay();
	// 생성 시 탄환 풀에 등록
	BulletPool.Add(this);
}

void ABoss_Attack1_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * DeltaTime;
		FHitResult HitResult;
		// Sweep 옵션을 활성화하여 이동 중 충돌 검사
		SetActorLocation(NewLocation, true, &HitResult);

		if (HitResult.bBlockingHit)
		{
			/*UE_LOG(LogTemp, Log, TEXT("Bullet sweep hit: %s at %s"),
				HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("Unknown"),
				*GetActorLocation().ToString());
			Explode();*/
			return;
		}

		TraveledDistance += BulletSpeed * DeltaTime;
		if (TraveledDistance >= MaxDistance)
		{
			Explode();
		}
	}
}


void ABoss_Attack1_Bullet::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	// 발사 위치 및 회전 설정
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);

	// 탄환 활성화 및 초기화
	bIsActive = true;
	TraveledDistance = 0.0f;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (Attack1Niagara)
	{
		Attack1Niagara->SetAutoActivate(true);
		Attack1Niagara->Activate();
	}

	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &ABoss_Attack1_Bullet::Explode, 10.0f, false);

	ABoss* BossRef = Cast<ABoss>(GetOwner());
	if (BossRef)
	{
		return;
	}
}

void ABoss_Attack1_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor->IsA(ABoss_Attack1_Bullet::StaticClass()))
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Bullet hit: %s"), *OtherActor->GetName());

	if (OtherActor->ActorHasTag("NPC") || OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Ground"))
	{
		Explode();
	}
}



void ABoss_Attack1_Bullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor->IsA(ABoss_Attack1_Bullet::StaticClass()))
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Bullet overlapped with: %s"), *OtherActor->GetName());

	if (OtherActor->ActorHasTag("NPC") || OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Ground"))
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			AActor* LocalOwner = OverlappedComp->GetOwner();
			ABoss_Attack1_Bullet* Bullet = Cast<ABoss_Attack1_Bullet>(LocalOwner);

			if (Bullet)
			{
				float DamageValue = 10.0f;
				UGameplayStatics::ApplyDamage(OtherActor, DamageValue, nullptr, Bullet, UDamageType::StaticClass());
			}
		}
		Explode();
	}
}


void ABoss_Attack1_Bullet::Explode()
{
	if (!bIsActive)
		return;

	GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);

	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (ExplosionEffect->Template)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect->Template, GetActorLocation());
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABoss_Attack1_Bullet::ResetBullet, ExplosionDelay, false);
}


void ABoss_Attack1_Bullet::ResetBullet()
{
	// 탄환 초기화
	bIsActive = false;
	TraveledDistance = 0.0f;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (Attack1Niagara)
	{
		Attack1Niagara->Deactivate();
		Attack1Niagara->SetAutoActivate(false);
	}

	if (!BulletPool.Contains(this))
	{
		BulletPool.Add(this);
	}
}

ABoss_Attack1_Bullet* ABoss_Attack1_Bullet::GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack1_Bullet> BulletClass)
{
	for (ABoss_Attack1_Bullet* Bullet : BulletPool)
	{
		if (Bullet && !Bullet->bIsActive)
		{
			return Bullet;
		}
	}
	if (World)
	{
		ABoss_Attack1_Bullet* NewBullet = World->SpawnActor<ABoss_Attack1_Bullet>(BulletClass);
		if (NewBullet)
		{
			BulletPool.Add(NewBullet);
			return NewBullet;
		}
	}
	return nullptr;
}

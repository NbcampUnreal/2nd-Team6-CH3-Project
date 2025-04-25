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

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->SetCollisionProfileName(TEXT("IgnoreAll"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->BodyInstance.bUseCCD = true;
	CollisionComp->OnComponentHit.AddDynamic(this, &ABoss_Attack1_Bullet::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Attack1_Bullet::OnOverlapBegin);
	RootComponent = CollisionComp;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	ExplosionEffect->SetupAttachment(RootComponent);
	ExplosionEffect->bAutoActivate = false;

	Attack1Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Attack1Niagara"));
	Attack1Niagara->SetupAttachment(RootComponent);
	Attack1Niagara->bAutoActivate = false;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}


void ABoss_Attack1_Bullet::BeginPlay()
{
	Super::BeginPlay();
	BulletPool.Add(this);
}

void ABoss_Attack1_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * DeltaTime;
		FHitResult HitResult;
		SetActorLocation(NewLocation, true, &HitResult);

		if (HitResult.bBlockingHit)
		{
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
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);

	bIsActive = true;
	TraveledDistance = 0.0f;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (Attack1Niagara)
	{
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
	bIsActive = false;
	TraveledDistance = 0.0f;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (Attack1Niagara)
	{
		Attack1Niagara->Deactivate();
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

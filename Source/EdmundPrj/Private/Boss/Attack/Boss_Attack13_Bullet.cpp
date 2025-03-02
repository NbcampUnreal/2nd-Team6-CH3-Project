#include "Boss/Attack/Boss_Attack13_Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

//TArray<ABoss_Attack13_Bullet*> ABoss_Attack13_Bullet::BulletPool;

ABoss_Attack13_Bullet::ABoss_Attack13_Bullet()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	// 스태틱 메시
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	// Hit 콜리전
	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComp);
	// 다른 액터와 겹칠 경우 호출될 함수 등록
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Attack13_Bullet::OnHitOverlap);

	// 폭발 콜리전
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->SetupAttachment(RootComp);

	// 파티클
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComp);
	ParticleComp->bAutoActivate = false;
	// 파티클 재생이 끝났을 때 호출될 델리게이트 등록
	ParticleComp->OnSystemFinished.AddDynamic(this, &ABoss_Attack13_Bullet::OnParticleSystemFinished);
}

void ABoss_Attack13_Bullet::BeginPlay()
{
	Super::BeginPlay();

	BossRef = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));

	//if (!BulletPool.Contains(this))
	//{
	//	BulletPool.Add(this);
	//}
}

void ABoss_Attack13_Bullet::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !OtherActor->IsA<ABoss_Attack13_Bullet>())
	{
		// 충돌한 액터의 이름을 출력합니다.
		UE_LOG(LogTemp, Warning, TEXT("Boss_Attack13_Bullet collided with: %s"), *OtherActor->GetName());

		// 총알 끼리 충돌을 무시하면서 겹쳐지게 합니다.
		if (MeshComp)
		{
			MeshComp->SetVisibility(false);  // 총알이 보이지 않게 처리할 수 있습니다.
		}
		SetActorEnableCollision(false);  // 충돌을 끄지 않고 물리적 충돌은 일어나지 않도록 합니다.
		if (ParticleComp)
		{
			ParticleComp->ActivateSystem(true);  // 파티클 재생
		}
	}
}



void ABoss_Attack13_Bullet::OnParticleSystemFinished(UParticleSystemComponent* PSystem)
{

	// 파티클 재생이 끝났을 때 액터 삭제
	Destroy();
}


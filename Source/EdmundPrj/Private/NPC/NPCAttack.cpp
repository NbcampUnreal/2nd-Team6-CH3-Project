// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPCAttack.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

// Sets default values
ANPCAttack::ANPCAttack()
{
    //UE_LOG(LogTemp, Warning, TEXT("공격 스폰"));

    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    SetRootComponent(SceneComp);

    // 박스 컴포넌트를 생성하고 씬 컴포넌트에 첨부
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    BoxComp->SetupAttachment(SceneComp);

    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ANPCAttack::OnOverlapAttackBegin);

    if (GetWorld())
    {
        FTimerHandle TimerHandle;
        this->GetWorldTimerManager().SetTimer(TimerHandle, this, &ANPCAttack::DestroyMe, 0.01f, false);

        // 이미 오버랩된 액터가 있는지 검사하고, 있다면 수동으로 이벤트 호출
        TArray<AActor*> OverlappingActors;
        BoxComp->GetOverlappingActors(OverlappingActors);

        for (AActor* Actor : OverlappingActors)
        {
            if (Actor && Actor->ActorHasTag(FName("Monster")))
            {
                OnOverlapAttackBegin(BoxComp, Actor, nullptr, 0, false, FHitResult());
            }
        }
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("GetWorld()가 없습니다."));
    }
}


void ANPCAttack::OnOverlapAttackBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Monster")))
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCAttack 몬스터에게 데미지를 가함"));

		UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageValue,
			nullptr,
			nullptr,
			UDamageType::StaticClass()
		);
	}
}

void ANPCAttack::DestroyMe()
{
	Destroy();
}



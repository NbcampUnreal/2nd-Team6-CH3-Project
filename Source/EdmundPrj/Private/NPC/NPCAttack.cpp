// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPCAttack.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

ANPCAttack::ANPCAttack()
{
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
}


void ANPCAttack::OnOverlapAttackBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Monster")))
	{
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



// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify_MeleeAttackCheck.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


void UAnimNotify_MeleeAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

    if (MeshComp && MeshComp->GetOwner())
    {
        AActor* Owner = MeshComp->GetOwner();
        ABaseMonster* Monster = Cast<ABaseMonster>(Owner);

        if (Monster)
        {
            Monster->MonsterAttackCheck();
        }
    }
}
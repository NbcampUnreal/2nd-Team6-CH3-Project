#include "NPC/NPCAIController.h"
#include "NPC/NPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ANPCAIController::ANPCAIController()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    NPCCharacter = Cast<ANPC>(InPawn);
}

void ANPCAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
}
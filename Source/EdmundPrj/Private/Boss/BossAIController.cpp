#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

ABossAIController::ABossAIController()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTree = nullptr;
}

void ABossAIController::BeginPlay()
{
    Super::BeginPlay();

    if (!BehaviorTree)
    {
        return;
    }

    if (!BlackboardComponent)
    {
        return;
    }

    UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
    RunBehaviorTree(BehaviorTree);

    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Player)
    {
        BlackboardComponent->SetValueAsObject("Player", Player);

        UObject* TestPlayer = BlackboardComponent->GetValueAsObject("Player");
    }
}





void ABossAIController::SetBehaviorTree(UBehaviorTree* NewBehaviorTree)
{
    if (NewBehaviorTree)
    {
        BehaviorTree = NewBehaviorTree;
        UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
        RunBehaviorTree(BehaviorTree);
    }
}
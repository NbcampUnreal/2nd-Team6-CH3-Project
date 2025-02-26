#include "NPC/NPCAIController.h"
#include "NPC/NPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ANPCAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    UE_LOG(LogTemp, Log, TEXT("NPCAIController::OnPossess Called"));

    NPCCharacter = Cast<ANPC>(InPawn);
    if (!NPCCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("NPCAIController::OnPossess - NPCCharacter is null"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("NPCAIController::OnPossess - Possessed NPC: %s"), *NPCCharacter->GetName());
    }
}

void ANPCAIController::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("NPCAIController::BeginPlay Called"));

    if (BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Log, TEXT("BehaviorTreeAsset is set. Running Behavior Tree..."));
        RunBehaviorTree(BehaviorTreeAsset);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BehaviorTreeAsset is NULL in NPCAIController!"));
    }
}

void ANPCAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (NPCCharacter)
    {
        UE_LOG(LogTemp, Verbose, TEXT("NPCAIController::Tick - NPCCharacter: %s, DeltaTime: %f"), *NPCCharacter->GetName(), DeltaTime);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NPCAIController::Tick - NPCCharacter is null"));
    }
}

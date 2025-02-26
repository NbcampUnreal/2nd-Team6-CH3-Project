#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPC/NPC.h"
#include "NPCAIController.generated.h"



UCLASS()
class EDMUNDPRJ_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ANPC* NPCCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTreeAsset;
//protected:
	//void Chase();
};

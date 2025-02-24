#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Skill3_Wall.generated.h"

class UStaticMeshComponent;

UCLASS()
class EDMUNDPRJ_API ABoss_Skill3_Wall : public AActor
{
    GENERATED_BODY()

public:
    ABoss_Skill3_Wall();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    FTimerHandle GravityTimerHandle;

    UFUNCTION()
    void SimulateGravity();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float GravityAcceleration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity")
    FVector CurrentVelocity;
};

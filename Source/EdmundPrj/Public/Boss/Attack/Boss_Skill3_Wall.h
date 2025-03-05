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

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 객체 풀을 위한 정적 변수
    static TArray<ABoss_Skill3_Wall*> WallPool;

    // 벽 활성화 상태
    bool bIsActive;

    // 풀에서 벽을 가져오는 함수
    static ABoss_Skill3_Wall* GetWallFromPool(UWorld* World, TSubclassOf<ABoss_Skill3_Wall> WallClass);

    // 벽을 활성화하는 함수 (위치, 회전 설정)
    void ActivateWall(FVector SpawnLocation, FRotator SpawnRotation);

    // 벽을 비활성화하고 풀로 반환하는 함수
    void DeactivateWall();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    // 중력 시뮬레이션 관련
    FTimerHandle GravityTimerHandle;
    UFUNCTION()
    void SimulateGravity();

    UFUNCTION()
    void LowerAndDeactivate();

    UFUNCTION()
    void PerformLowering();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float GravityAcceleration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity")
    FVector CurrentVelocity;

    FTimerHandle LowerTimerHandle;
    float LowerTargetZ;
};

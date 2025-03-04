// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Materials\MaterialInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyDirectionArrow.generated.h"

class ASupportCharacter;
class ABaseCharacter;
class ABaseMonster;

UCLASS()
class EDMUNDPRJ_API AEnemyDirectionArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyDirectionArrow();

public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Setting")
	TObjectPtr<USceneComponent> SceneComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Setting")
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyDirection|Component")
	TObjectPtr<UMaterialInterface> CurrentMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyDirection|Component")
	TObjectPtr<UMaterialInterface> DefaultMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyDirection|Component")
	TObjectPtr<UMaterialInterface> DangerMaterial = nullptr;

	TObjectPtr<ABaseMonster> Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyDirection")
	TObjectPtr<ASupportCharacter> SupportChar;

	float CollisionRadius;
};

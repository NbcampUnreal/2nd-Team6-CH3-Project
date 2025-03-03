// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMissionItem.generated.h"

class UBoxComponent;
class UWidgetComponent;
class AMissionHandle;
class UInteractionWidget;

UCLASS()
class EDMUNDPRJ_API ABaseMissionItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseMissionItem();
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type);
	virtual void ActionEventByPressedKey();
	virtual void SetIsActive(bool Value);

	void SetMissionText(const FString& NewInfoText, const FString& NewActiveText, const FString& NewClearText);
	void SetMissionStory(const bool Value, const bool BeginValue, const int32 Index);
	bool GetIsContainStory() const;
	void PrintMissionInfoText();
	void PrintMissionActiveText();
	void PrintMissionClearText();
	void PrintBeginMissionStory();
	void PrintEndMissionStory();
	
protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;
	
	virtual void ActionBeginOverlap();
	virtual void ActionEndOverlap();
	virtual void ActionOnHit();
	virtual void CompleteProgress();

	void SetVisible(bool bIsVisible);

	void ApplyOverlapCollision(bool bIsBlockedMesh);
	void ApplyBlockCollision();

	void UpdateMissionTextToUI(const FString& TargetText, bool bIsClear = false);
	void UpdateNotifyTextToUI(const FString& TargetText);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass = nullptr;

protected:
	TObjectPtr<AMissionHandle> MissionHandle;
	TObjectPtr<UInteractionWidget> InteractionWidget;
	FName MissionType;
	FString MissionInfoText;
	FString MissionActiveText;
	FString MissionClearText;

	bool bIsContainStory = false;
	bool bIsBegin = false;
	int32 StoryIndex = 0;

	bool bIsPlayingInteraction = false;
	bool bIsActive = false;
	float ProgressValue = 1.0f;
};

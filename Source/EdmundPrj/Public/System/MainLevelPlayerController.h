// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainLevelPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class EDMUNDPRJ_API AMainLevelPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MainLevelIMC = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LeftClickAction = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void InputLeftMouseButtonClick();
	void CheckCollideCharacter();
};

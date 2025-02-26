// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UCanvasPanel;

UCLASS()
class EDMUNDPRJ_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateProgressBar(float Value);
	void UpdateKeyText(FString InputKeyString);
	void UpdateInfoText(FString KeyInfoString);
	void VisibleNotify(bool bIsVisible);
	void VisibleProgressBar(bool bIsVisible);


private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> NotifyBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ProgressBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> InfoText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
};

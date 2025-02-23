// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class USkillListWidget;

UCLASS()
class EDMUNDPRJ_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSkill(USkillListWidget* ListWidget, int32 SkillNum);
	void UpdateSkill(UTexture2D* SkillImage, const FName& SkillName);
	void InvisibleBorder();
	const int32 GetSkillNumber() const;

private:
	UFUNCTION()
	void OnClickedButton();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectSkillButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillNameText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> Border;

	TObjectPtr<USkillListWidget> SkillListWidget;
	int32 SkillNumber = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "SkillListWidget.generated.h"

class UUIHandle;
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class EDMUNDPRJ_API USkillListWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public: // GameState가 스킬 리스트 계산하면 Notify로 값 전달, GameInstance가 AddViewport 호출
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void EndRemoveAnim() override;

private:
	UFUNCTION()
	void OnClickedSelectButton();

	UFUNCTION()
	void OnClickedSkillButton1();

	UFUNCTION()
	void OnClickedSkillButton2();

	UFUNCTION()
	void OnClickedSkillButton3();

	void OnSelectedImage(int32 Index);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectButton1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectButton2;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectButton3;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> Border1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> Border2;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> Border3;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillName1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillName2;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillName3;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseAnim;

	//TArray<>
};

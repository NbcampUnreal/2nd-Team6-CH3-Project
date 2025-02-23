// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "SkillListWidget.generated.h"

class UUIHandle;
class USkillWidget;
class UButton;
class UImage;
class UTextBlock;
class UUniformGridPanel;

UCLASS()
class EDMUNDPRJ_API USkillListWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public: // GameState가 스킬 리스트 계산하면 Notify로 값 전달, GameInstance가 AddViewport 호출
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void PlayAddAnim() override;
	virtual void EndRemoveAnim() override;

	void OnClickedSkillButton(USkillWidget* SkillWidget);

private:
	UFUNCTION()
	void OnClickedSelectButton();

	void CreateSkillWidget();
	void InvisibleAllBorder();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USkillWidget> BaseSkill;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SkillList;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseAnim;

	TArray<TObjectPtr<USkillWidget>> SkillElements;
	TSubclassOf<USkillWidget> SkillWidgetClass = nullptr;
	TObjectPtr<USkillWidget> SelectedSkill = nullptr;
};

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
	
public: // GameState�� ��ų ����Ʈ ����ϸ� Notify�� �� ����, GameInstance�� AddViewport ȣ��
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

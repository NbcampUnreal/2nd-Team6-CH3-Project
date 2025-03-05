// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "ResultWidget.generated.h"

class UUIHandle;
class UButton;
class UTextBlock;

UCLASS()
class EDMUNDPRJ_API UResultWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public: // add notify
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void ChangedMissionStateToEnd(const int32 StateMoney, const int32 InstanceMoney, const int32 MissionMoney) override;
	virtual void ChangedIsGameClear(const bool bIsClear) override;

private: 
	UFUNCTION()
	void OnClickedRetry();

	virtual void EndAddAnim() override;

	void UpdateResult(bool bIsClear);
	void DecreaseCurrentMoneyCount();
	void DecreaseMissionMoneyCount();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveNextButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveMainButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveNextButtonText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText2;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText3;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	int32 TempCurrentMoney = 0;
	int32 TempTotalMoney = 0;
	int32 TempMissionMoney = 0;

	float DecreaseTime = 100;

	FTimerHandle Timer;
};

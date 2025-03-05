// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UResultWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;

	Super::InitWidget(NewUIHandle);

	MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	MoveMainButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMain);
}

void UResultWidget::ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp)
{
	Super::ChangedPlayerHp(MaxHp, CurrentHp);

	if (CurrentHp <= 0)
	{
		UpdateResult(false);
	}
	else
	{
		UpdateResult(true);
	}
}

void UResultWidget::ChangedMissionStateToEnd(const int32 StateMoney, const int32 InstanceMoney, const int32 MissionMoney)
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);

	TempCurrentMoney = StateMoney;
	TempTotalMoney = InstanceMoney;
	TempMissionMoney = MissionMoney;

	ValueText1->SetText(FText::FromString(FString::FromInt(TempCurrentMoney)));
	ValueText2->SetText(FText::FromString(FString::FromInt(TempMissionMoney)));
	ValueText3->SetText(FText::FromString(FString::FromInt(TempTotalMoney)));

	TempTotalMoney += TempCurrentMoney + TempMissionMoney;
}

void UResultWidget::UpdateResult(bool bIsClear)
{
	MoveNextButton->OnClicked.Clear();

	if (bIsClear)
	{
		MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
		TitleText->SetText(FText::FromString(FString::Printf(TEXT("Mission Clear"))));
		MoveNextButtonText->SetText(FText::FromString(FString::Printf(TEXT("다음으로"))));
	}
	else
	{
		MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedRetry);
		TitleText->SetText(FText::FromString(FString::Printf(TEXT("Mission Fail"))));
		MoveNextButtonText->SetText(FText::FromString(FString::Printf(TEXT("재도전"))));
	}
}

void UResultWidget::OnClickedRetry()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedRetry();
}

void UResultWidget::EndAddAnim()
{
	Super::EndAddAnim();
	ValueText3->SetText(FText::FromString(FString::FromInt(TempTotalMoney)));
	//GetWorld()->GetTimerManager().SetTimer(Timer, this, &ThisClass::DecreaseCurrentMoneyCount, DecreaseTime, true);
}

void UResultWidget::DecreaseCurrentMoneyCount()
{
	if (TempCurrentMoney <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &ThisClass::DecreaseMissionMoneyCount, DecreaseTime, true);
	}

	--TempCurrentMoney;
	++TempTotalMoney;

	ValueText1->SetText(FText::FromString(FString::FromInt(TempCurrentMoney)));
	ValueText3->SetText(FText::FromString(FString::FromInt(TempTotalMoney)));
}

void UResultWidget::DecreaseMissionMoneyCount()
{
	if (TempMissionMoney <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}

	--TempMissionMoney;
	++TempTotalMoney;

	ValueText2->SetText(FText::FromString(FString::FromInt(TempMissionMoney)));
	ValueText3->SetText(FText::FromString(FString::FromInt(TempTotalMoney)));
}


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

void UResultWidget::UpdateResult(bool bIsClear)
{
	// 게임 결과 텍스트들 출력하도록 구현 필요
	MoveNextButton->OnClicked.Clear();

	if (bIsClear)
	{
		MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
		TitleText->SetText(FText::FromString(FString::Printf(TEXT("Mission Clear"))));
		MoveNextButtonText->SetText(FText::FromString(FString::Printf(TEXT("Move Next"))));
	}
	else
	{
		MoveNextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedRetry);
		TitleText->SetText(FText::FromString(FString::Printf(TEXT("Mission Fail"))));
		MoveNextButtonText->SetText(FText::FromString(FString::Printf(TEXT("Retry"))));
	}
}

void UResultWidget::OnClickedRetry()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedRetry();
}


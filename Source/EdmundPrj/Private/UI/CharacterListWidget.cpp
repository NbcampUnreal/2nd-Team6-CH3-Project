// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterListWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"

void UCharacterListWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedSelect);
	CancleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCancle);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedClose);
}

void UCharacterListWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();
	UIHandle->RemoveCoverFromViewport(EWidgetType::CharacterListWidget);
}

void UCharacterListWidget::OnClickedSelect()
{
	// 선택 된 것 없으면 비활성화 기능 구현해야함
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	UIHandle->ClickedSelectCharacter(CharacterType);
	OnClickedCloseWidget(EWidgetType::CharacterListWidget);
}

void UCharacterListWidget::OnClickedCancle()
{
	//자리로 돌아가게 하기
	SelectedCharacter = nullptr;
	CharacterType = ECharacterType::Gunner;
}

void UCharacterListWidget::OnClickedClose()
{
	OnClickedCloseWidget(EWidgetType::CharacterListWidget);
}

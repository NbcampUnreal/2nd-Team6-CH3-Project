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

void UCharacterListWidget::PlayAddAnim()
{
	Super::PlayAddAnim();
	SetEnableButton(true);
}

void UCharacterListWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();
	UIHandle->RemoveCoverFromViewport(EWidgetType::CharacterListWidget);
}

void UCharacterListWidget::ChangedCharacterType(const ECharacterType CharacterType)
{
	Super::ChangedCharacterType(CharacterType);
	TargetCharacterType = CharacterType;
	SetEnableButton(true);

	UEnum* CharacterEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterType"));
	FString TypeName = CharacterEnum->GetNameStringByValue((int64)CharacterType);
	UE_LOG(LogTemp, Warning, TEXT("Selected Character Type is %s"), *TypeName);
}

void UCharacterListWidget::OnClickedSelect()
{
	//SetEnableButton(false);
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	UIHandle->ClickedSelectCharacter(TargetCharacterType);
	OnClickedCloseWidget(EWidgetType::CharacterListWidget);
}

void UCharacterListWidget::OnClickedCancle()
{
	SetEnableButton(false);
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	UIHandle->ClickedCancleSelectCharacter();
	TargetCharacterType = ECharacterType::Gunner;
}

void UCharacterListWidget::OnClickedClose()
{
	OnClickedCloseWidget(EWidgetType::CharacterListWidget);
}

void UCharacterListWidget::SetEnableButton(bool bIsEnable)
{
	SelectButton->SetIsEnabled(bIsEnable);
	CancleButton->SetIsEnabled(bIsEnable);
}

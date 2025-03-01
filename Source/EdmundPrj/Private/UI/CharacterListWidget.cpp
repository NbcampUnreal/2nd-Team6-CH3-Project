// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterListWidget.h"
#include "System/UIHandle.h"
#include "System/DataStructure/CharacterDataRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCharacterListWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedSelect);
	CancleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCancle);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedClose);

	CharacterData = UIHandle->GetCharacterData();
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
	PrintCharacterInfo();
	/*UEnum* CharacterEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterType"));
	FString TypeName = CharacterEnum->GetNameStringByValue((int64)CharacterType);
	UE_LOG(LogTemp, Warning, TEXT("Selected Character Type is %s"), *TypeName);*/
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

	CharacterNameText->SetText(FText::FromString(FString::Printf(TEXT(""))));
	CharacterInfoText->SetText(FText::FromString(FString::Printf(TEXT(""))));
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

void UCharacterListWidget::PrintCharacterInfo()
{
	for (const FCharacterDataRow* CharacterDataRow : CharacterData)
	{
		if (CharacterDataRow->CharacterType == TargetCharacterType)
		{
			CharacterInfoText->SetText(FText::FromString(FString::Printf(TEXT(""))));
			CharacterNameText->SetText(FText::FromString(CharacterDataRow->CharacterName.ToString()));

			TArray<FString> ParsingString;
			CharacterDataRow->CharacterInfo.ToString().ParseIntoArray(ParsingString, TEXT("_"));

			for (const FString& TargetString : ParsingString)
			{
				FString Origin = CharacterInfoText->GetText().ToString();
				CharacterInfoText->SetText(FText::FromString(FString::Printf(TEXT("%s \n %s"), *Origin, *TargetString)));
			}
			break;
		}
	}
}

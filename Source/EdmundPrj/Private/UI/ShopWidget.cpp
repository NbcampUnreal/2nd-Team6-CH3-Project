// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShopWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "System/DataStructure/ShopCatalogRow.h"
#include "UI/SubWidget/AdvanceWidget.h"
#include "UI/SubWidget/ShopInfoWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/UniformGridPanel.h"

void UShopWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	InitShop();

	CheckBuyBox->SetVisibility(ESlateVisibility::Collapsed);
	NotifyBox->SetVisibility(ESlateVisibility::Collapsed);

	AgreeButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedAgree);
	CancleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCancle);
	NotifyCloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedNotifyClose);
	ShopCloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedShopClose);
}

void UShopWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();
	UIHandle->RemoveCoverFromViewport(EWidgetType::ShopWidget);
}

void UShopWidget::OnClickedBuyButton(UAdvanceWidget* AdvanceWidget)
{
	CloseAgreeBox();
	OnClickedNotifyClose();

	TargetElement = AdvanceWidget;
	OpenAgreeCheck();
}

void UShopWidget::OnClickedAgree()
{
	FName TargetName = TargetElement->GetElementName();
	const FShopCatalogRow* SelectRow = UIHandle->ClickedBuyAgree(TargetName, UpdateValue);

	if (SelectRow == nullptr)
	{
		OpenNotify(false);
	}
	else
	{
		OpenNotify(true);
		UpdateStates(SelectRow);
	}

	CloseAgreeBox();
}

void UShopWidget::OnClickedCancle()
{
	CloseAgreeBox();
}

void UShopWidget::OnClickedNotifyClose()
{
	NotifyBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UShopWidget::OnClickedShopClose()
{
	CloseAgreeBox();
	OnClickedNotifyClose();

	OnClickedCloseWidget(EWidgetType::ShopWidget);
}

void UShopWidget::OpenAgreeCheck()
{
	FName TargetName = TargetElement->GetElementName();
	FText SelectedText = FText::FromString(FString::Printf(TEXT("Buy %s?"), *TargetName.ToString()));
	SelectedElementText->SetText(SelectedText);
	CheckBuyBox->SetVisibility(ESlateVisibility::Visible);
}

void UShopWidget::CloseAgreeBox()
{
	TargetElement = nullptr;
	CheckBuyBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UShopWidget::OpenNotify(bool bIsSuccess)
{
	if (bIsSuccess)
	{
		ResultText->SetText(FText::FromString(FString::Printf(TEXT("Completed"))));
	}
	else
	{
		ResultText->SetText(FText::FromString(FString::Printf(TEXT("Insufficient"))));
	}

	NotifyBox->SetVisibility(ESlateVisibility::Visible);
}

void UShopWidget::UpdateStates(const FShopCatalogRow* ResultRow)
{
	if (ResultRow->CurrentLevel == ResultRow->MaxLevel)
	{
		TargetElement->ApplyMaxLevel();
	}
	else
	{
		TargetElement->UpdatePrice(ResultRow->CurrentPrice);
	}

	MoneyText->SetText(FText::FromString(FString::FromInt(UIHandle->GetCurrentMoney())));
	PairAdvanceAndInfo[TargetElement]->UpdateInfoValue(ResultRow->AdvanceValue * ResultRow->CurrentLevel);
}

void UShopWidget::InitShop()
{
	const TArray<FShopCatalogRow*> CurrentAdvance = UIHandle->GetCurrentAdvance();

	ElementClass = BaseElement.GetClass();
	InfoClass = BaseInfo.GetClass();
	MoneyText->SetText(FText::FromString(FString::FromInt(UIHandle->GetCurrentMoney())));

	for (int32 i = 0; i < CurrentAdvance.Num(); i++)
	{
		InitElements(CurrentAdvance[i]);
		InitInfo(CurrentAdvance[i], i);
	}
}

void UShopWidget::InitInfo(const FShopCatalogRow* RowData, int32 Index)
{
	TObjectPtr<UShopInfoWidget> NewElement = CreateWidget<UShopInfoWidget>(this, InfoClass);

	int32 RowIndex = Index / 2;
	int32 ColumnIndex = Index % 2;

	InfoContents->AddChildToUniformGrid(NewElement, RowIndex, ColumnIndex);
	InfoElements.Add(NewElement);

	NewElement->InitInfo(RowData->SkillImage, RowData->AdvanceName);
	NewElement->UpdateInfoValue(RowData->AdvanceValue * RowData->CurrentLevel);

	PairAdvanceAndInfo.Add(Elements[Index], InfoElements[Index]);
}

void UShopWidget::InitElements(const FShopCatalogRow* RowData)
{
	TObjectPtr<UAdvanceWidget> NewElement = CreateWidget<UAdvanceWidget>(this, ElementClass);

	Contents->AddChild(NewElement);
	Elements.Add(NewElement);

	NewElement->InitWidget(RowData->AdvanceName, this);
	NewElement->UpdateInfo(RowData->InfoString);
	NewElement->UpdateSkillImage(RowData->SkillImage);

	if (RowData->CurrentLevel == RowData->MaxLevel)
	{
		NewElement->ApplyMaxLevel();
	}
	else
	{
		NewElement->UpdatePrice(RowData->CurrentPrice);
	}
}
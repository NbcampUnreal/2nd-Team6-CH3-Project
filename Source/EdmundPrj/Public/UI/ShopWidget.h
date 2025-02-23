// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "ShopWidget.generated.h"

class UUIHandle;
class UButton;
class UTextBlock;
class UCanvasPanel;
class UAdvanceWidget;
class UShopInfoWidget;
class UVerticalBox;
class UUniformGridPanel;
struct FShopCatalogRow;

UCLASS()
class EDMUNDPRJ_API UShopWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void EndRemoveAnim() override;
	void OnClickedBuyButton(UAdvanceWidget* AdvanceWidget);

private: 
	UFUNCTION()
	void OnClickedAgree(); // 가격 체크, 상태 갱신

	UFUNCTION()
	void OnClickedCancle();

	UFUNCTION()
	void OnClickedNotifyClose();

	UFUNCTION()
	void OnClickedShopClose();

	void InitShop();
	void InitInfo(const FShopCatalogRow* RowData, int32 Index);
	void InitElements(const FShopCatalogRow* RowData);
	void OpenAgreeCheck();
	void CloseAgreeBox();
	void OpenNotify(bool bIsSuccess);
	void UpdateStates(const FShopCatalogRow* ResultRow);

private :
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UVerticalBox> Contents;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InfoContents;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAdvanceWidget> BaseElement;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UShopInfoWidget> BaseInfo;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> AgreeButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CancleButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> NotifyCloseButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> ShopCloseButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedElementText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CheckBuyBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> NotifyBox;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseAnim;

	TArray<TObjectPtr<UAdvanceWidget>> Elements;
	TArray<TObjectPtr<UShopInfoWidget>> InfoElements;
	TMap<TObjectPtr<UAdvanceWidget>, TObjectPtr<UShopInfoWidget>> PairAdvanceAndInfo;

	TObjectPtr<UAdvanceWidget> TargetElement;
	TSubclassOf<UAdvanceWidget> ElementClass;
	TSubclassOf<UShopInfoWidget> InfoClass;
	int32 UpdateValue = 1;
};

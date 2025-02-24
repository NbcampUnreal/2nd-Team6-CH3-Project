// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "CharacterListWidget.generated.h"

class UUIHandle;
class UButton;

UCLASS()
class EDMUNDPRJ_API UCharacterListWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void PlayAddAnim() override;
	virtual void EndRemoveAnim() override;
	//virtual void ChangedClickedCharacter(AActor* Character) override;

private: // 선택한 캐릭터 타입 반환 필요, 캐릭터 선택 기능 구현
	UFUNCTION()
	void OnClickedSelect();
	
	UFUNCTION()
	void OnClickedCancle();

	UFUNCTION()
	void OnClickedClose();

	void SetEnableButton(bool bIsEnable);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CancleButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseAnim;

	ECharacterType CharacterType = ECharacterType::Gunner;
	TObjectPtr<AActor> SelectedCharacter = nullptr;
};

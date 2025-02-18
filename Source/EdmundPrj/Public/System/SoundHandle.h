// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundHandle.generated.h"

class UEdmundGameInstance;

UCLASS()
class EDMUNDPRJ_API USoundHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitSoundHandle(UEdmundGameInstance* NewGameInstance);

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance;
};

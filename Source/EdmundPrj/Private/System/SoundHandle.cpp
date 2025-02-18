// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SoundHandle.h"
#include "System/EdmundGameInstance.h"

//void USoundHandle::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	
//}

void USoundHandle::InitSoundHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;
}

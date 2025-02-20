// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DataHandle.h"
#include "System/EdmundGameInstance.h"

//void UDataHandle::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	
//}

void UDataHandle::InitDataHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;
}

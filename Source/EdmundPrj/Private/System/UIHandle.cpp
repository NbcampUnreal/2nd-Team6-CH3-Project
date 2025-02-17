// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UIHandle.h"

void UUIHandle::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Init UIHandle"));
}

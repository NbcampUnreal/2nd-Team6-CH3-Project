// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossTestCharacter.h"

// Sets default values
ABossTestCharacter::ABossTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


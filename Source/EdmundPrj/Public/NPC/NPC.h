#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

UCLASS()
class EDMUNDPRJ_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

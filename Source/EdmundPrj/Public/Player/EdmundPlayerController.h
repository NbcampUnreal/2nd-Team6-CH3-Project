#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EdmundPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class EDMUNDPRJ_API AEdmundPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AEdmundPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction = nullptr;

protected:
	virtual void BeginPlay() override;
};

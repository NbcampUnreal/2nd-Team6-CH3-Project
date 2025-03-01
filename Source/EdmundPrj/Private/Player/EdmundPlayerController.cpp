#include "Player/EdmundPlayerController.h"
#include "EnhancedInputSubsystems.h"

AEdmundPlayerController::AEdmundPlayerController()
{
}

void AEdmundPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 카메라 시점 높낮이 제한
	PlayerCameraManager->ViewPitchMin = -45;
	PlayerCameraManager->ViewPitchMax = 45;

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

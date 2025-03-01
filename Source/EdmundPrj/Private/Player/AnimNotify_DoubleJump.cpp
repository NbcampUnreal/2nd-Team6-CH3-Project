#include "Player/AnimNotify_DoubleJump.h"
#include "Player/PlayerCharacterFey.h"

void UAnimNotify_DoubleJump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		APlayerCharacterFey* Player = Cast<APlayerCharacterFey>(Owner);

		if (Player)
		{
			Player->JumpCount = 1;
		}
	}
}

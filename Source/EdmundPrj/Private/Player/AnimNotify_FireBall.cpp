#include "Player/AnimNotify_FireBall.h"
#include "Player/PlayerCharacterFey.h"

void UAnimNotify_FireBall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		APlayerCharacterFey* Player = Cast<APlayerCharacterFey>(Owner);

		if (Player)
		{
			Player->ActiveWeapon();
		}
	}
}

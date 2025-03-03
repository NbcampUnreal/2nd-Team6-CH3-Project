#include "Player/AnimNotify_Reload.h"
#include "Player/PlayerCharacter.h"

void UAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);

		if (Player)
		{
			Player->PlayReloadSound();
		}
	}
}

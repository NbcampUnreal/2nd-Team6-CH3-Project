#include "Player/AnimNotify_AuroraAttackCombo.h"
#include "Player/PlayerCharacterAurora.h"

void UAnimNotify_AuroraAttackCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		APlayerCharacterAurora* Player = Cast<APlayerCharacterAurora>(Owner);

		if (Player)
		{
			Player->NextCombo();
		}
	}
}

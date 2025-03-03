#include "Player/AnimNotify_EndAttack.h"
#include "Player/PlayerCharacter.h"

void UAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);

		if (Player)
		{
			Player->SetIsAttack();
		}
	}
}

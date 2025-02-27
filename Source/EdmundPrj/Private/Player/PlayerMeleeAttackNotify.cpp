#include "Player/PlayerMeleeAttackNotify.h"
#include "Player/BaseCharacter.h"

void UPlayerMeleeAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		ABaseCharacter* Player = Cast<ABaseCharacter>(Owner);

		if (Player)
		{
			Player->AttackTrace();
		}
	}
}

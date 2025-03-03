#include "Player/AnimNotify_Bullet.h"
#include "Player/PlayerCharacter.h"

void UAnimNotify_Bullet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);

		if (Player)
		{
			Player->ActiveWeapon();
		}
	}
}

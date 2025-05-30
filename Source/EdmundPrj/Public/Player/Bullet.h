#pragma once

#include "CoreMinimal.h"
#include "Player/BaseProjectile.h"
#include "Bullet.generated.h"

class USphereComponent;

UCLASS()
class EDMUNDPRJ_API ABullet : public ABaseProjectile
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};

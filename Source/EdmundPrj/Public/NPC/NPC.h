#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

class UCapsuleComponent;
class UMeshComponent;
struct FInputActionValue;

UCLASS()
class EDMUNDPRJ_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	UCapsuleComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	UMeshComponent* MeshComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	bool bIsRescue; // 구출 상태

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float HP; // 현재 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float MaxHP; // 최대 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float WalkSpeed; // 걷기 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float AttackDamage; // 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	int32 Defense; // 방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	float AttackSpeed; // 공격 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	int32 CurrentAmmo; // 탄환 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	int32 MaxAmmo; // 최대 탄환 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	int32 CriticalProb; // 치명타 확률

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	int32 EvasionProb; // 회피 확률

	void Move();
	void Reload();
	void Attack();
	void MeleeAttack();
	void TakeDamage(float Damage);
	void AmountAmmo(float AmountAmmo);
	void OnDeath();
	

	float GetHP() const { return HP; };
	void SetHP(const float NewHP);
	void SetAmmo(const int32 NewAmmo);

	
};

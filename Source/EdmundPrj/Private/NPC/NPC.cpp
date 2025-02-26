#include "NPC/NPC.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = GetCapsuleComponent();
	MeshComp = GetMesh();

	bIsRescue = false;
	MaxHP = 500.0f;
	HP = MaxHP;
	WalkSpeed = 300.0f;
	AttackDamage = 50.0f;
	Defense = 10;
	AttackSpeed = 1.0f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	CriticalProb = 5;
	EvasionProb = 5;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ANPC::Move()
{

}

void ANPC::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void ANPC::Attack()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
	else
	{
		Reload();
	}
}

void ANPC::MeleeAttack()
{
}

void ANPC::TakeDamage(float Damage)
{
	float EffectiveDamage = Damage * (1.0f - (Defense / 100.0f));
	HP = FMath::Max(0.0f, HP - EffectiveDamage);
	if (HP <= 0)
	{
		OnDeath();
	}
}

void ANPC::AmountAmmo(float AmountAmmo)
{
	int32 AmmoToAdd = FMath::RoundToInt(AmountAmmo);
	CurrentAmmo = FMath::Clamp(CurrentAmmo + AmmoToAdd, 0, MaxAmmo);
}

void ANPC::OnDeath()
{
	Destroy();
}

void ANPC::SetHP(const float NewHP)
{
	HP = FMath::Clamp(NewHP, 0.0f, MaxHP);
}

void ANPC::SetAmmo(const int32 NewAmmo)
{
	CurrentAmmo = FMath::Clamp(NewAmmo, 0, MaxAmmo);
}
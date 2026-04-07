  // Copyright Epic Games, Inc. All Rights Reserved.

#include "FT_ReloadingAndAmmoProjectile.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include <TargetPanel.h>
#include "ProjectileParticleFX.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/StaticMeshActor.h"

AFT_ReloadingAndAmmoProjectile::AFT_ReloadingAndAmmoProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFT_ReloadingAndAmmoProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 6000;
	ProjectileMovement->MaxSpeed = 6000;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Die after 3 seconds by default
	InitialLifeSpan = 0;
}
void AFT_ReloadingAndAmmoProjectile::setStats(int BulletType)
{
	if (BulletType == 2) {
		ProjectileMovement->MaxSpeed = 3000;
	}
	else if (BulletType == 1){
		ProjectileMovement->MaxSpeed = 3000;
	}
}

void AFT_ReloadingAndAmmoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ATargetPanel* target = Cast<ATargetPanel>(OtherActor);
		AStaticMeshActor* targetBackup = Cast<AStaticMeshActor>(OtherActor);
		if (target)
		{
			target->Player = PlayerPointer;
			target->Hit(Damage);
			Destroy();
		}
		else if (ProjectileFX && targetBackup) {
			//spawns the particle FX
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileFX, GetActorLocation(), GetActorRotation());
			Destroy();
		} 
	}
}
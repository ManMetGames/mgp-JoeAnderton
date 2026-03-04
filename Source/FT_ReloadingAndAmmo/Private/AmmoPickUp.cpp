// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickUp.h"
#include "../TP_WeaponComponent.h" 

// Sets default values
AAmmoPickUp::AAmmoPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	RootComponent = PickUpMesh;

	// Set up overlap events
	PickUpMesh->SetGenerateOverlapEvents(true);
	PickUpMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickUp::OnOverlap);		// call the hti function when we hit something
}

// Called when the game starts or when spawned
void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPickUp::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Ammo PickUp hit by: %s"), *OtherActor->GetName());
	// check if the other actor is a character and if so give them ammo
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Other Actor is valid"));
		// check if the other actor has a weapon component and if so give them ammo
		UTP_WeaponComponent* WeaponComp = OtherActor ->FindComponentByClass<UTP_WeaponComponent>();
		// search through all the child actors of the other actor to find a weapon component if we didn't find one on the other actor itself
		if (!WeaponComp)
		{
			TArray<AActor*> AttachedActors;
			OtherActor->GetAttachedActors(AttachedActors);
			for (AActor* Child : AttachedActors)
			{
				WeaponComp = Child->FindComponentByClass<UTP_WeaponComponent>();
				if (WeaponComp)
					break;
			}
		}

		if (WeaponComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Other Actor has a weapon component"));
			WeaponComp->SetNewAmmoType(AmmoType);
			Destroy();
		}
	}
}


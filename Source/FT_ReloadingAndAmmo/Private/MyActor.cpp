// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "../FT_ReloadingAndAmmoCharacter.h" 
#include "Components/StaticMeshComponent.h" 

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character!=nullptr && !teleported)
		teleport();
}

void AMyActor::teleport() {
	UE_LOG(LogTemp, Warning, TEXT("Huh"));
	FVector Destination = Character->GetActorLocation() - FVector(100, 0, 0);
	Character->SetActorLocation(GetActorLocation());
	SetActorLocation(Destination);
	teleported = true;
}


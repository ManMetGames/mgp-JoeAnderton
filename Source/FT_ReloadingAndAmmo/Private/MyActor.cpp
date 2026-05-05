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

	//This code is unused, I just couldn't figure out how to delete the file
}


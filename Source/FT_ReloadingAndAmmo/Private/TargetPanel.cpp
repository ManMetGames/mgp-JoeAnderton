// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetPanel.h"
#include "Components/StaticMeshComponent.h" 
#include "Math/UnrealMathUtility.h"  

// Sets default values
ATargetPanel::ATargetPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	RootComponent = TargetMesh;

	// Set up overlap events
	TargetMesh->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ATargetPanel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATargetPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATargetPanel::Hit(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Target hit with damage: %d"), Damage);
	bIsHit = true;
	//swaps the location of the player and the block
	if (Player != nullptr) {
		FVector Destination = Player->GetActorLocation();
		Player->SetActorLocation(GetActorLocation()+FVector(0,0,50));
		SetActorLocation(Destination+FVector(0,0,-50));
	}
}
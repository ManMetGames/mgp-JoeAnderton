// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetPanel.h"
#include "Components/StaticMeshComponent.h" 
#include "Kismet/GameplayStatics.h"
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
	bIsHit = false;
	// add a random direction for the target to move in if bIsMoving is true
	if (bIsMoving)
	{		
		if (FMath::RandRange(0, 1) == 0)
		{
			MoveDirection = 1;
		}
		else
		{
			MoveDirection = -1;
		}
	}
}

// Called every frame
void ATargetPanel::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
	TargetMesh->SetMaterial(0, OffMaterial); //automatically changes material back to default if no raycast
}

void ATargetPanel::Hit()
{
	if (Player != nullptr) {
		//only teleports the player if the target isn't disabled
		switch (CurrentState)
		{
		case TargetState::Enabled:
			FVector Destination = Player->GetActorLocation();
			Player->SetActorLocation(GetActorLocation() + FVector(0, 0, 50));
			SetActorLocation(Destination + FVector(0, 0, -50));
			if (TeleportSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, Player->GetActorLocation());
			}
			break;
		case TargetState::Disabled:
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("ENUMS broke!"));
			break;
		}
	}
}

void ATargetPanel::ChangeTarget()
{
	//raycast changes the target colour to yellow or red depending if it's enabled or disabled
	switch (CurrentState)
	{
	case TargetState::Enabled:
		TargetMesh->SetMaterial(0, OnMaterial);
		break;
	case TargetState::Disabled:
		TargetMesh->SetMaterial(0, DisabledMaterial);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("ENUMS broke!"));
		break;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetPanel.generated.h"

UCLASS()
class FT_RELOADINGANDAMMO_API ATargetPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetPanel();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	bool bIsHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;

	int MoveDirection = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true")) 
	class UStaticMeshComponent* TargetMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle ResetTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void Hit(int Damage);
	UFUNCTION()
	void ResetTarget();
};

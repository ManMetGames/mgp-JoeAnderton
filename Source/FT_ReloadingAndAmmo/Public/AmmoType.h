// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AmmoType.generated.h"

/**
 * 
 */
UCLASS()
class FT_RELOADINGANDAMMO_API UAmmoType : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public: 
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
        FString AmmoName;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
        int AmmoCount;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
        int Damage;

};

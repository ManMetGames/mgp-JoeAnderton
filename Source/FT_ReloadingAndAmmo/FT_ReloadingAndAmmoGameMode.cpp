// Copyright Epic Games, Inc. All Rights Reserved.

#include "FT_ReloadingAndAmmoGameMode.h"
#include "FT_ReloadingAndAmmoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFT_ReloadingAndAmmoGameMode::AFT_ReloadingAndAmmoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

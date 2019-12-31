// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RemakeCatFestivalGameMode.h"
#include "RemakeCatFestivalHUD.h"
#include "RemakeCatFestivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARemakeCatFestivalGameMode::ARemakeCatFestivalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARemakeCatFestivalHUD::StaticClass();
}

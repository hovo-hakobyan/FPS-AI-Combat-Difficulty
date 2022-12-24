// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_AccuracyGameMode.h"
#include "AI_AccuracyHUD.h"
#include "AI_AccuracyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAI_AccuracyGameMode::AAI_AccuracyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAI_AccuracyHUD::StaticClass();
}

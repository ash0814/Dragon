// Copyright Epic Games, Inc. All Rights Reserved.

#include "DragonGameMode.h"
#include "DragonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADragonGameMode::ADragonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/P_BulePrint/BP_CPlayer.BP_CPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

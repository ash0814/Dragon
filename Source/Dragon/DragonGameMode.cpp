// Copyright Epic Games, Inc. All Rights Reserved.

#include "DragonGameMode.h"
#include "DragonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADragonGameMode::ADragonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

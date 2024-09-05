// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GameMode.h"
#include "C_MainUI.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "C_Crystal.h"
#include "C_Player.h"
#include "C_Weapon.h"
#include "C_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"


AC_GameMode::AC_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/P_BulePrint/BP_CPlayer.BP_CPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AC_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AC_GameMode::BeginPlay"));

	CurrentState = EGameState::Ready;
	MainUI = CreateWidget<UC_MainUI>(GetWorld(), MainUIWidgetFactory);
	if (MainUI != nullptr)
		MainUI->AddToViewport();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	// Get Total Crystal Actor Count
	TArray<AActor*> CrystalActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_Crystal::StaticClass(), CrystalActors);
	TotalCrystalCount = CrystalActors.Num();
}

void AC_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_GameMode::GameOver()
{
	MainUI->OnGameOver();
	//// get player
	//AC_Player *player = Cast<AC_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//if (player != nullptr)
	//{
	//	// player weapon off
	//	player->WeaponComp->End_Fire();
	//}
	//GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	//GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

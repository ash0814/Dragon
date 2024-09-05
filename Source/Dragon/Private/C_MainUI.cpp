// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MainUI.h"
#include "Components/Button.h"
#include "C_GameMode.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

void UC_MainUI::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_GameStart = Cast<UButton>(GetWidgetFromName(TEXT("Btn_GameStart")));
	ReadyCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ReadyCanvas")));
	GamePlayingCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("GamePlayingCanvas")));
	DestroyCrystalDone = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("DestroyCrystalDone")));
	GameOverCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("GameOverCanvas")));
	GameClearCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("GameClearCanvas")));
	PlayerCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("PlayerCanvas")));
	if (GamePlayingCanvas != nullptr)
		GamePlayingCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (GameOverCanvas != nullptr)
		GameOverCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (PlayerCanvas != nullptr)
		PlayerCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (GameClearCanvas != nullptr)
		GameClearCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (Btn_GameStart != nullptr)
		Btn_GameStart->OnClicked.AddDynamic(this, &UC_MainUI::OnClickGameStart);
}

void UC_MainUI::OnClickGameStart()
{
	AC_GameMode* GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->CurrentState = EGameState::Start;
	}
	ReadyCanvas->SetVisibility(ESlateVisibility::Hidden);
	GamePlayingCanvas->SetVisibility(ESlateVisibility::Visible);
	PlayerCanvas->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	OnGameStart();
}

void UC_MainUI::OnChangeCrystalCountZero()
{
	DestroyCrystalDone->SetActiveWidgetIndex(1);
}

void UC_MainUI::OnGameOver()
{
	GamePlayingCanvas->SetVisibility(ESlateVisibility::Hidden);
	PlayerCanvas->SetVisibility(ESlateVisibility::Hidden);
	GameOverCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UC_MainUI::OnGameClear()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Game Clear"));
	GamePlayingCanvas->SetVisibility(ESlateVisibility::Hidden);
	PlayerCanvas->SetVisibility(ESlateVisibility::Hidden);
	GameClearCanvas->SetVisibility(ESlateVisibility::Visible);
}



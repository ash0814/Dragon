// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MainUI.h"
#include "Components/Button.h"
#include "C_GameMode.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UC_MainUI::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_GameStart = Cast<UButton>(GetWidgetFromName(TEXT("Btn_GameStart")));
	ReadyCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ReadyCanvas")));
	GamePlayingCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("GamePlayingCanvas")));
	DestroyCrystalDone = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("DestroyCrystalDone")));
	GameOverCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("GameOverCanvas")));
	PlayerCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("PlayerCanvas")));
	if (GamePlayingCanvas != nullptr)
		GamePlayingCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (GameOverCanvas != nullptr)
		GameOverCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (PlayerCanvas != nullptr)
		PlayerCanvas->SetVisibility(ESlateVisibility::Hidden);
	if (Btn_GameStart != nullptr)
		Btn_GameStart->OnClicked.AddDynamic(this, &UC_MainUI::OnClickGameStart);
}

void UC_MainUI::OnClickGameStart()
{
	AC_GameMode* GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnClickGameStart"));
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("OnChangeCrystalCountZero"));
	DestroyCrystalDone->SetActiveWidgetIndex(1);
}

void UC_MainUI::OnGameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("OnGameOver"));
	GamePlayingCanvas->SetVisibility(ESlateVisibility::Hidden);
	PlayerCanvas->SetVisibility(ESlateVisibility::Hidden);
	GameOverCanvas->SetVisibility(ESlateVisibility::Visible);

}



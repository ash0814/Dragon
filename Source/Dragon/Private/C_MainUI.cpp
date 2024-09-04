// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MainUI.h"
#include "Components/Button.h"
#include "C_GameMode.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

void UC_MainUI::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_GameStart = Cast<UButton>(GetWidgetFromName(TEXT("Btn_GameStart")));
	ReadyCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ReadyCanvas")));
	if (Btn_GameStart != nullptr)
	{
		Btn_GameStart->OnClicked.AddDynamic(this, &UC_MainUI::OnClickGameStart);
	}
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
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}

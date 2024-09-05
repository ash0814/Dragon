// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_MainUI.generated.h"

/**
 * 
 */
UCLASS()
class DRAGON_API UC_MainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_GameStart;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* ReadyCanvas;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* GamePlayingCanvas;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* DestroyCrystalDone;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* GameOverCanvas;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* GameClearCanvas;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* PlayerCanvas;

	UFUNCTION()
	void OnClickGameStart();

	UFUNCTION(BlueprintCallable)
	void OnChangeCrystalCountZero();

	UFUNCTION(BlueprintCallable)
	void OnGameOver();

	UFUNCTION(BlueprintCallable)
	void OnGameClear();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStart();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "C_GameMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameState : uint8
{
	Ready,
	Start,
	Playing,
	Pause,
	GameOver
};

UCLASS()
class DRAGON_API AC_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AC_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameState")
	EGameState CurrentState = EGameState::Ready;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainUIWidgetFactory;

	UPROPERTY()
	class UC_MainUI* MainUI;

	void ChangeState(EGameState NewState);

	void Ready();
	void Start();
	void Playing();
	void Pause();
	void GameOver();



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Enemy.h"
#include "C_Enemy_Boss.generated.h"

/**
 * 
 */
UCLASS()
class DRAGON_API AC_Enemy_Boss : public AC_Enemy
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AC_Enemy_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Die() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=EnemyBossData)
	float MoveSpeed = 1000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=EnemyBossData)
	float currentTime = 0;
};

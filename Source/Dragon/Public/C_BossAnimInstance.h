// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_BossAnimInstance.generated.h"
 
UENUM(BlueprintType)
enum class EBossState : uint8
{
	Walk,
	Run,
	Fly,
	Attack,
	Die
};


UCLASS()
class DRAGON_API UC_BossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class AC_Enemy_Boss* OwnerBoss;
	class UCharacterMovementComponent* MovementCom;
	//Player 의 현재 Speed 값
	void PlayerCurrentSpeed();
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Boss Data")
	EBossState State;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss Data")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss Data")
	float Pitch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss Data")
	float Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss Data")
	bool bIsShouldMove;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss Data")
	bool bIsShouldFly;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss Data")
	bool bBossIsRun;
};

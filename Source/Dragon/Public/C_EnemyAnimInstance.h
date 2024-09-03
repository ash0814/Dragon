// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_EnemyFSM.h"
#include "C_EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DRAGON_API UC_EnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EEnemyState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EEnemyState PrevState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDamaged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRoaring;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsIdle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsFlying;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsLanding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsTakingOff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHovering;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringBackward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringRotateLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringRotateRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringStrafeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHoveringStrafeRight;
};

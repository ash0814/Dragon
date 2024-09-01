// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_TestBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DRAGON_API UC_TestBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=TestVal)
	float speed;
};

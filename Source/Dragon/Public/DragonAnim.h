// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DragonAnim.generated.h"

/**
 * 
 */
UCLASS()
class DRAGON_API UDragonAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=DragonAnim)
	float speed = 0;
};

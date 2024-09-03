// Fill out your copyright notice in the Description page of Project Settings.


#include "C_EnemyAnimInstance.h"

void UC_EnemyAnimInstance::OnEndAttackAnimation()
{
    bAttackPlay = false;
}

void UC_EnemyAnimInstance::PlayDamageAnim(FName sectionName)
{
}

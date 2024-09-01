// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BossAnimInstance.h"
#include "C_Enemy_Boss.h"
#include "CHelpers.h"


void UC_BossAnimInstance::NativeBeginPlay()
{
	//OwnerBoss = Cast<AC_Enemy_Boss>(TryGetPawnOwner());

	OwnerBoss = Cast<AC_Enemy_Boss>(GetSkelMeshComponent()->GetOwner());
	if (OwnerBoss == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("There is No BOSS"));
	MovementCom = OwnerBoss->GetCharacterMovement();

}

void UC_BossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	PlayerCurrentSpeed();
}

void UC_BossAnimInstance::PlayerCurrentSpeed()
{
	if (OwnerBoss)
		Speed = OwnerBoss->GetVelocity().Size2D();
	//else
	//	UE_LOG(LogTemp, Warning, TEXT("There is No Owner"));

}



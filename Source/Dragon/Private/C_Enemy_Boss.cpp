// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Enemy_Boss.h"

AC_Enemy_Boss::AC_Enemy_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Project/Asset/FourEvilDragonsPBR/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	//ABP วาด็
	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Boss/B_Blueprints/Animation/ABP_CEnemy_Boss.ABP_CEnemy_Boss_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
	
}

void AC_Enemy_Boss::BeginPlay()
{
}

void AC_Enemy_Boss::Tick(float DeltaTime)
{
	FVector newLocation = GetActorLocation() + (GetActorForwardVector() * DeltaTime * MoveSpeed);
	SetActorLocation(newLocation);
}

void AC_Enemy_Boss::Die()
{
}

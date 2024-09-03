// Fill out your copyright notice in the Description page of Project Settings.


#include "C_TESTLOG.h"

// Sets default values
AC_TESTLOG::AC_TESTLOG()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_TESTLOG::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("TEST"));
	
}

// Called every frame
void AC_TESTLOG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


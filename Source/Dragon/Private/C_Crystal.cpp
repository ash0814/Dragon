// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Crystal.h"

// Sets default values
AC_Crystal::AC_Crystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TotalHP = 100;
	CurrentHP = 100;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SetRootComponent(SphereMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
}

// Called when the game starts or when spawned
void AC_Crystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Crystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


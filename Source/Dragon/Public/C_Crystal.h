// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Crystal.generated.h"

UCLASS()
class DRAGON_API AC_Crystal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Crystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Crystal")
	float TotalHP;

	UPROPERTY(EditAnywhere, Category="Crystal")
	float CurrentHP;

	// sphere static mesh
	UStaticMeshComponent* SphereMesh;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_EnemyFSM.h"
#include "C_Enemy.generated.h"

UCLASS()
class DRAGON_API AC_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float MaxHP;
	UPROPERTY(EditAnywhere, Category = "Enemy")
	float CurrentHP;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	bool bCanHurt = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	UC_EnemyFSM* EnemyFSM;

	void GetHurt(float Amount);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

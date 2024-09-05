// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Player.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*)


UCLASS()
class DRAGON_API AC_Player : public ACharacter
{
	GENERATED_BODY()

public:
	AC_Player();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 헤더 파일 (TestPlayer.h)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = "WeaponComp")
	class UC_WeaponComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	float CurrentHP;

	// 입력
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imc_TPS;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Lookup;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Turn
		;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_fly;

private:
	//좌우 입력 처리
	void Turn(const struct FInputActionValue& inputValue);
	//상하 회전 입력
	void LookUp(const struct FInputActionValue& inputValue);
	void Turn(float value);
	void LookUp(float value);
	void Move(const struct FInputActionValue& inputValue);

	//점프 입력 처리 함수
	void InputJump(const struct FInputActionValue& inputValue);
	void PlayerMove();

	void Fly(const struct FInputActionValue& inputValue);

public:
	FInputBindingDelegate OnInputBindingDelegate;

private:
	float walkSpeed = 600;
	FVector direction;

public:
	bool bIsWeapon;
	bool bIsRun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool bIsFlying = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	bool bCanFly = true;   // 플레이어가 날 수 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	USoundBase* TakeDamageSound;

private:
	float FlightTime = 5.0f;   // 날 수 있는 최대 시간 (초)
	float CooldownTime = 3.0f; // 다시 날기까지 기다려야 하는 쿨타임 시간 (초)

	FTimerHandle FlightTimerHandle;  // 플라이 시간 타이머
	FTimerHandle CooldownTimerHandle; // 쿨타임 타이머

	void StopFlying();          // 날기를 중단하는 함수
	void ResetFlyAbility();     // 다시 날 수 있는 상태로 초기화하는 함수
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageSound();
};


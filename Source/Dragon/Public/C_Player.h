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

	UPROPERTY(VisibleAnywhere, Category = "WeaponComp")
	class UC_WeaponComponent* WeaponComp;

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

};


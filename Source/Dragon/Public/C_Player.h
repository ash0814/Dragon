// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Player.generated.h"

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
	// ��� ���� (TestPlayer.h)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// �Է�
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imc_TPS;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Lookup;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Turn;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_equip;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_fire;

	//�¿� �Է� ó��
	void Turn(const struct FInputActionValue& inputValue);
	//���� ȸ�� �Է�
	void LookUp(const struct FInputActionValue& inputValue);
	void Turn(float value);
	void LookUp(float value);
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Move;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;
	FVector direction;
	void Move(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_jump;
	//���� �Է� ó�� �Լ�
	void InputJump(const struct FInputActionValue& inputValue);
	void PlayerMove();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_fly;
	void Fly(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "WeaponComp")
	class UC_WeaponComponent* WeaponComp;


	//void PlayerModeChange();
	//void PlayerBegin_Fire();
	//void PlayerEnd_Fire();

public:
	bool bIsWeapon;
	bool bIsRun;

};


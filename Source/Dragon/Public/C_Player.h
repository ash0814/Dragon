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
	// ��� ���� (TestPlayer.h)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "WeaponComp")
	class UC_WeaponComponent* WeaponComp;
	// �Է�
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
	//�¿� �Է� ó��
	void Turn(const struct FInputActionValue& inputValue);
	//���� ȸ�� �Է�
	void LookUp(const struct FInputActionValue& inputValue);
	void Turn(float value);
	void LookUp(float value);
	void Move(const struct FInputActionValue& inputValue);

	//���� �Է� ó�� �Լ�
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CurrentHP;

private:
	bool bIsFlying = false;
	bool bCanFly = true;        // �÷��̾ �� �� �ִ��� ����
	float FlightTime = 5.0f;   // �� �� �ִ� �ִ� �ð� (��)
	float CooldownTime = 3.0f; // �ٽ� ������� ��ٷ��� �ϴ� ��Ÿ�� �ð� (��)

	FTimerHandle FlightTimerHandle;  // �ö��� �ð� Ÿ�̸�
	FTimerHandle CooldownTimerHandle; // ��Ÿ�� Ÿ�̸�

	void StopFlying();          // ���⸦ �ߴ��ϴ� �Լ�
	void ResetFlyAbility();     // �ٽ� �� �� �ִ� ���·� �ʱ�ȭ�ϴ� �Լ�

	void AnyDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};


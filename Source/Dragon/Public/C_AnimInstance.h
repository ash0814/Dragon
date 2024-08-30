#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_WeaponComponent.h"

#include "C_AnimInstance.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Forward,
	Right,
	Backward,
	Left,
};

UCLASS()
class DRAGON_API UC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected: //Player Data
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Data")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Data")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Data")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Data")
	bool bIsShouldMove;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Data")
	bool bPlayerIsRun;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Data")
	bool bPlayerIsWeapon;

protected: //Set OrientationAngle
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Orientation Angle")
	float F_OrientationAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Orientation Angle")
	float R_OrientationAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Orientation Angle")
	float B_OrientationAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Orientation Angle")
	float L_OrientationAngle;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MoveDirection")
	EMovementType MovementType = EMovementType::Forward;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	EWeaponType WeaponType = EWeaponType::MAX;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	//Player �� ���� Speed ��
	void PlayerCurrentSpeed();
	//Player�� ���� Run �������� Ȯ��
	void PlayerCurrentRun();
	//Player�� ���� Weapon�� ������ �ִ��� Ȯ��
	void PlayerCurrentWeapon();

	//4���� Direction�� ���� EMovementType ����
	void DirectionCalculation();
	//���� �����̰� �ִ��� �Ǵ�
	void ShouldMove();
	//Orientation �����Լ�
	void SetOrientationAngle();

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);


private: //Math���� �Ⱥҷ������� ���� ��ü �Լ�
	//NormalizeAxis ��ü �Լ�
	float NormalizeAngle(float Angle);

private: //class Value
	class AC_Player* OwnerCharacter;
	class UCharacterMovementComponent* MovementCom;
	class UC_WeaponComponent* Weapon;


};

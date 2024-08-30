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
	//Player 의 현재 Speed 값
	void PlayerCurrentSpeed();
	//Player가 현재 Run 상태인지 확인
	void PlayerCurrentRun();
	//Player가 현재 Weapon을 가지고 있는지 확인
	void PlayerCurrentWeapon();

	//4방향 Direction에 따라서 EMovementType 변경
	void DirectionCalculation();
	//현재 움직이고 있는지 판단
	void ShouldMove();
	//Orientation 설정함수
	void SetOrientationAngle();

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);


private: //Math에서 안불러와져서 만든 대체 함수
	//NormalizeAxis 대체 함수
	float NormalizeAngle(float Angle);

private: //class Value
	class AC_Player* OwnerCharacter;
	class UCharacterMovementComponent* MovementCom;
	class UC_WeaponComponent* Weapon;


};

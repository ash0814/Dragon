#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "C_Player.generated.h"

UCLASS()
class DRAGON_API AC_Player : public ACharacter
{
	GENERATED_BODY()

private: //Player Camera Limit
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchRange = FVector2D(-40, +40);
	/////////////////////////////////////////////////////////////////

public://Player Base Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UTimelineComponent* TimelineCom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UC_WeaponComponent* WeaponComp;
	/////////////////////////////////////////////////////////////////

public: //INPUT COMPONENT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;	//InputMappinContext

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Move_Player;	//Movement

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Look_Player;	//Look

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Run_Player; //Run

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Equip; //Equip

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Fire; //Bullet Fire
	/////////////////////////////////////////////////////////////////

private:
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* ZoomCurve;


public: //생성자
	AC_Player();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected://Player Move Input
	void InputMove(const FInputActionValue& values);
	void InputLook(const FInputActionValue& values);

private:
	UFUNCTION()
	void CameraZoom(float alpha);

	void SetTimeline();

protected://Player Move Func
	void OnRun();
	void OffRun();

public://Local Value
	//현재 Run인지 아닌지 check
	bool bIsRun;

	//Curve 조건 확인
	bool bIsIdle;

	bool bIsWeapon;

private:
	//Timeline Update에 대한 델리게이트
	FOnTimelineFloat CameraZoomHandler;
	float DefaultSpringArmLength;
	float ZoomedSpringArmLength;

};

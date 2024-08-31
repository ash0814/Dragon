#include "C_AnimInstance.h"
#include "CHelpers.h"

#include "C_Player.h"
#include "C_WeaponComponent.h"

#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UC_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<AC_Player>(TryGetPawnOwner());
	CheckNUll(OwnerCharacter);

	Weapon = Cast<UC_WeaponComponent>(OwnerCharacter->GetComponentByClass(UC_WeaponComponent::StaticClass()));
	CheckNUll(Weapon);

	//WeaponComponent의 Delegate를 받아서 WeaponType이 변경되는 것을 반영함
	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UC_AnimInstance::OnWeaponTypeChanged);

	MovementCom = OwnerCharacter->GetCharacterMovement();

}

void UC_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter == nullptr)
		return;

	//Set Speed
	PlayerCurrentSpeed();
	PlayerCurrentRun();

	//Set Direciton
	DirectionCalculation();

	//Set Aim Offset Pitch
	SetDirectionPitch();

	//Set Orientation Angle
	SetOrientationAngle();

	//Set IsShouldMove
	ShouldMove();


}

void UC_AnimInstance::PlayerCurrentSpeed()
{
	Speed = OwnerCharacter->GetVelocity().Size2D();
}

void UC_AnimInstance::PlayerCurrentRun()
{
	bPlayerIsRun = OwnerCharacter->bIsRun;
}

void UC_AnimInstance::PlayerCurrentWeapon()
{
	bPlayerIsWeapon = OwnerCharacter->bIsWeapon;
}

void UC_AnimInstance::DirectionCalculation()
{
	FVector dir = FVector(OwnerCharacter->GetVelocity().X, OwnerCharacter->GetVelocity().Y, 0);

	float calcul = UAnimInstance::CalculateDirection(dir, OwnerCharacter->GetActorRotation());

	Direction = NormalizeAngle(calcul);

	if (Direction > -45.0f && Direction <= 45.0f)
	{
		MovementType = EMovementType::Forward;
	}
	else if (Direction > 45.0f && Direction <= 135.0f)
	{
		MovementType = EMovementType::Right;
	}
	else if (Direction > 135.0f || Direction <= -135.0f)
	{
		MovementType = EMovementType::Backward;
	}
	else if (Direction > -135.0f && Direction <= -45.0f)
	{
		MovementType = EMovementType::Left;
	}


}

void UC_AnimInstance::ShouldMove()
{
	FVector Accel = MovementCom->GetCurrentAcceleration();

	if (Speed > 3.0f && Accel != FVector(0.0f, 0.0f, 0.0f))
		bIsShouldMove = true;
	else
		bIsShouldMove = false;
}

void UC_AnimInstance::SetOrientationAngle()
{
	F_OrientationAngle = Direction - 0.0f;
	R_OrientationAngle = Direction - 90.0f;
	B_OrientationAngle = Direction - 180.0f;
	L_OrientationAngle = Direction - (-90.0f);
}

void UC_AnimInstance::SetDirectionPitch()
{
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, GetDeltaSeconds(), 25);

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, GetDeltaSeconds(), 25);
}

void UC_AnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

float UC_AnimInstance::NormalizeAngle(float Angle)
{
	return fmod(Angle + 180.0f, 360.0f) - 180.0f;
}
#include "C_Player.h"
#include "CHelpers.h"

#include "C_AnimInstance.h"
#include "C_WeaponComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AC_Player::AC_Player()
{
	//Create Components
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		TimelineCom = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimeline"));
		WeaponComp = CreateDefaultSubobject<UC_WeaponComponent>(TEXT("WeaponComponent"));
	}

	//Create RootComponents
	{
		SpringArm->SetupAttachment(ACharacter::GetMesh());
		Camera->SetupAttachment(SpringArm);
	}

	//Set Default Class & Object Setting
	{
		//Set Skeletal Mesh
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

			if (SkMesh.Succeeded())
				ACharacter::GetMesh()->SetSkeletalMesh(SkMesh.Object);
		}

		//Set Animinstance
		{
			static ConstructorHelpers::FClassFinder<UAnimInstance> animInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/P_BulePrint/Animation/ABP_C_Player.ABP_C_Player_C'"));

			if (animInstance.Succeeded())
				ACharacter::GetMesh()->SetAnimClass(animInstance.Class);
		}

		//Set Cureve
		{
			static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Script/Engine.CurveFloat'/Game/Player/P_CameraShake/C_Camera.C_Camera'"));

			if (Curve.Succeeded())
				ZoomCurve = Curve.Object;
		}

		//Set InputAction
		{
			//Input Move
			{
				static ConstructorHelpers::FObjectFinder<UInputAction> Move(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/P_Input/IA_Move.IA_Move'"));

				if (Move.Succeeded())
					IA_Move_Player = Move.Object;
			}

			//Input Look
			{
				static ConstructorHelpers::FObjectFinder<UInputAction> Look(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/P_Input/IA_Look.IA_Look'"));

				if (Look.Succeeded())
					IA_Look_Player = Look.Object;
			}

			//Input Run
			{
				static ConstructorHelpers::FObjectFinder<UInputAction> Run(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/P_Input/IA_Run.IA_Run'"));

				if (Run.Succeeded())
					IA_Run_Player = Run.Object;
			}

			//Input Gun
			{
				static ConstructorHelpers::FObjectFinder<UInputAction> Gun(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/P_Input/IA_Equip.IA_Equip'"));

				if (Gun.Succeeded())
					IA_Equip = Gun.Object;
			}
		}
	}

	//Player Setting
	{
		//Mesh Location, Rotaion Setting
		ACharacter::GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		ACharacter::GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

		//SpringArm, Camera Setting
		SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
		SpringArm->TargetArmLength = 200.0f;
		SpringArm->SocketOffset = FVector(0.0f, 50.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraLag = true;

		DefaultSpringArmLength = 200.0f;
		ZoomedSpringArmLength = 300.0f;


		//Player Movement Setting
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		bIsRun = false;
	}
}

void AC_Player::BeginPlay()
{
	Super::BeginPlay();

	//Player Camera View Up/Down Limit
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMin = PitchRange.X;
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMax = PitchRange.Y;

	SetTimeline();
}

void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimelineCom->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

//Player Input Function
void AC_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Set InputMappingContext
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	//Set InputAction binding

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Player Moving
		EnhancedInputComponent->BindAction(IA_Move_Player, ETriggerEvent::Triggered, this, &AC_Player::InputMove);
		//Player Looking
		EnhancedInputComponent->BindAction(IA_Look_Player, ETriggerEvent::Triggered, this, &AC_Player::InputLook);
		//Player Run
		EnhancedInputComponent->BindAction(IA_Run_Player, ETriggerEvent::Started, this, &AC_Player::PlayerRun);
		//Player Weapon
		EnhancedInputComponent->BindAction(IA_Equip, ETriggerEvent::Started, WeaponComp, &UC_WeaponComponent::SetAK47Mode);
	}

}

void AC_Player::InputMove(const FInputActionValue& values)
{
	//IA���� �Է°��� Axis2D�� �޾ƿͼ� Vector2D�� ����
	FVector2D MovementVector = values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//���� ���� ���ϱ�
		const FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);

		//���� Vector ���ϱ�
		const FVector ForwardDirection = FQuat(rotator).GetForwardVector();

		//�¿� Vector ���ϱ�
		const FVector RightDirection = FQuat(rotator).GetRightVector();

		//���� Vector�� ������� Add Movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AC_Player::InputLook(const FInputActionValue& values)
{
	FVector2D LookAxisVector = values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//Player Yaw, Pitch Input
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

//Camera Timeline Function
void AC_Player::CameraZoom(float alpha)
{
	if (bIsRun == true)
	{
		float ArmLength = FMath::Lerp(DefaultSpringArmLength, ZoomedSpringArmLength, alpha);
		SpringArm->TargetArmLength = ArmLength;

		float RunOffset = FMath::Lerp(50.0f, 0.0f, alpha);
		SpringArm->SocketOffset.Y = RunOffset;
	}
	else if (bIsRun == false)
	{

		float ArmLength = FMath::Lerp(DefaultSpringArmLength, ZoomedSpringArmLength, alpha);
		SpringArm->TargetArmLength = ArmLength;

		float RunOffset = FMath::Lerp(50.0f, 0.0f, alpha);
		SpringArm->SocketOffset.Y = RunOffset;
	}
}

void AC_Player::SetTimeline()
{
	if (ZoomCurve)
	{
		CameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
		TimelineCom->AddInterpFloat(ZoomCurve, CameraZoomHandler);
		TimelineCom->SetLooping(false);

		CameraZoom(TimelineCom->GetPlaybackPosition());
	}
}
///////////////////////////////////////////////////////////////////////////////////////////

//Local Function
void AC_Player::PlayerRun()
{

	if (GetVelocity().Size() > 0.0f)
	{
		bIsRun = !bIsRun;

		if (bIsRun == true)
		{
			TimelineCom->Play();
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}
		else if (bIsRun == false)
		{
			TimelineCom->Reverse();
			GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		}
	}
}

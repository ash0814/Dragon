	// Fill out your copyright notice in the Description page of Project Settings.


	#include "C_Player.h"
	#include <GameFramework/SpringArmComponent.h>
	#include <Camera/CameraComponent.h>
	#include "EnhancedInputSubsystems.h"
	#include "EnhancedInputComponent.h"
	#include "InputActionValue.h"
    #include "GameFramework/CharacterMovementComponent.h"
	#include "GameFramework/Character.h"

	// Sets default values
	AC_Player::AC_Player()
	{
		// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

		//스켈레탈메시 데이터 불러오기
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
		if (TempMesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(TempMesh.Object);
			//메쉬 컴포너트의 위치를 설정
			GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		}
		//SpringArm 컴포너트 붙이기
		springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeRotation(FRotator(0, 70, 90));
		springArmComp->TargetArmLength = 400;
		springArmComp->bUsePawnControlRotation = true;

		//camera 컴포너트 붙이기
		tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
		tpsCamComp->SetupAttachment(springArmComp);
		tpsCamComp->bUsePawnControlRotation = false;

		bUseControllerRotationYaw = true;
	}

	// Called when the game starts or when spawned
	void AC_Player::BeginPlay()
	{
		
		Super::BeginPlay();
		auto pc = Cast<APlayerController>(Controller);
		if (pc)
		{
			auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
			if (subsystem)
			{
				subsystem->AddMappingContext(imc_TPS, 0);
			}
		}
	}

	void AC_Player::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

		PlayerMove();

	}


	// Called to bind functionality to input
	void AC_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);

		auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		if (PlayerInput)
		{
			PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &AC_Player::Turn);
			PlayerInput->BindAction(ia_Lookup, ETriggerEvent::Triggered, this, &AC_Player::LookUp);
			PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &AC_Player::Move);
			PlayerInput->BindAction(ia_jump, ETriggerEvent::Started, this, &AC_Player::InputJump);
			PlayerInput->BindAction(ia_fly, ETriggerEvent::Triggered, this, &AC_Player::Fly);

		}
	}

	void AC_Player::Turn(const FInputActionValue& inputValue)
	{

		float value = inputValue.Get<float>();
		AddControllerYawInput(value);
	}

	void AC_Player::LookUp(const FInputActionValue& inputValue)
	{
		float value = inputValue.Get<float>();
		AddControllerPitchInput(value);
	}
	void AC_Player::Move(const FInputActionValue& inputValue)
	{
		// 3D 벡터로 변경하여 X, Y, Z 축 모두 처리
		FVector value = inputValue.Get<FVector>();

		// X, Y 축 방향 처리
		direction.X = value.X;
		direction.Y = value.Y;
		// Z 축 방향 (Fly 입력에 따른 위/아래 이동)
		direction.Z = value.Z; // Z 값은 Fly에서 설정될 것
	}
	void AC_Player::InputJump(const FInputActionValue& inputValue)
	{

		Jump();
	}
	void AC_Player::PlayerMove()
	{
		// X, Y 축 이동 처리 (회전값에 따라 방향 변환)
		FVector XYDirection = FVector(direction.X, direction.Y, 0.0f);
		FVector transformedXYDirection = FTransform(GetControlRotation()).TransformVector(XYDirection);

		// Z 축 이동은 별도로 처리하여 회전 변환 없이 적용
		FVector ZDirection = FVector(0.0f, 0.0f, direction.Z);

		// 최종 이동 벡터 계산
		FVector FinalMovement = transformedXYDirection + ZDirection;

		// 이동 처리
		AddMovementInput(FinalMovement);

		// 로그로 이동 벡터 출력 (디버깅용)
		UE_LOG(LogTemp, Warning, TEXT("FinalMovement -> X: %f, Y: %f, Z: %f"), FinalMovement.X, FinalMovement.Y, FinalMovement.Z);

		// 매 프레임마다 direction 초기화
		direction = FVector::ZeroVector;
	}

	void AC_Player::Fly(const FInputActionValue& inputValue)
	{
		// Z축 방향 이동 값을 설정 (양수: 위로, 음수: 아래로)
		float flyValue = inputValue.Get<float>();

		// Z축 이동 속도를 고려하여 값 설정
		direction.Z = flyValue * walkSpeed;

		// Shift 키 등으로 비행할 때 비행 모드로 전환하고 중력 비활성화
		if (flyValue > 0.0f)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->GravityScale = 0.0f;
			GetCharacterMovement()->BrakingDecelerationFlying = 2048.0f; // 비행 시 감속 속도
		}
		// Ctrl 키로 하강할 때 중력 활성화 및 걷기 모드로 전환
		else if (flyValue < 0.0f)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCharacterMovement()->GravityScale = 1.0f;
			GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f; // 걷기 모드에서의 감속 속도
		}

		// 로그로 입력 값과 Z축 이동 값 출력
		UE_LOG(LogTemp, Warning, TEXT("Fly -> Input Value: %f, Calculated Z: %f"), flyValue, direction.Z);
	}


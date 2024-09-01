	// Fill out your copyright notice in the Description page of Project Settings.


	#include "C_Player.h"
	#include <GameFramework/SpringArmComponent.h>
	#include <Camera/CameraComponent.h>
	#include "EnhancedInputSubsystems.h"
	#include "EnhancedInputComponent.h"
	#include "InputActionValue.h"
    #include "GameFramework/CharacterMovementComponent.h"


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

		// 방향 벡터와 속도, DeltaTime을 이용하여 이동 벡터 계산 (X, Y, Z 모두 포함)
		FVector Movement = direction * walkSpeed * DeltaTime;

		// 캐릭터의 현재 위치를 가져옴
		FVector CurrentLocation = GetActorLocation();

		// 최종 위치를 계산
		FVector NewLocation = CurrentLocation + Movement;

		// 계산된 위치로 캐릭터를 이동
		SetActorLocation(NewLocation);

		// 이동 벡터를 로그로 출력하여 확인
		UE_LOG(LogTemp, Warning, TEXT("Movement -> X: %f, Y: %f, Z: %f"), Movement.X, Movement.Y, Movement.Z);

		// 매 프레임마다 direction 초기화
		direction = FVector::ZeroVector;
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
		// 전체 방향 벡터를 한 번에 처리 (X, Y, Z 모두 포함)
		FVector FinalDirection = FVector(direction.X, direction.Y, direction.Z);

		// 카메라의 회전값을 고려하여 벡터를 변환
		FVector TransformedDirection = FTransform(GetControlRotation()).TransformVector(FinalDirection);

		// 최종 이동 처리
		AddMovementInput(TransformedDirection);

		// 이동 벡터를 로그로 출력
		UE_LOG(LogTemp, Warning, TEXT("TransformedDirection -> X: %f, Y: %f, Z: %f"), TransformedDirection.X, TransformedDirection.Y, TransformedDirection.Z);

		// 매 프레임마다 direction 초기화
		direction = FVector::ZeroVector;
	}


	void AC_Player::Fly(const FInputActionValue& inputValue)
	{
		// Z축 방향 이동 값을 설정 (양수: 위로, 음수: 아래로)
		float flyValue = inputValue.Get<float>();

		// Z축 이동 속도를 고려하여 값 설정
		direction.Z = flyValue;

		// 입력 값과 계산된 Z축 값 로그 출력
		//UE_LOG(LogTemp, Warning, TEXT("Input Value: %f, Calculated Z: %f"), flyValue, direction.Z);
	}

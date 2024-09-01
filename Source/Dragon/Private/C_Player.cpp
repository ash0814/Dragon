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

		//���̷�Ż�޽� ������ �ҷ�����
		ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
		if (TempMesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(TempMesh.Object);
			//�޽� ������Ʈ�� ��ġ�� ����
			GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		}
		//SpringArm ������Ʈ ���̱�
		springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->SetRelativeRotation(FRotator(0, 70, 90));
		springArmComp->TargetArmLength = 400;
		springArmComp->bUsePawnControlRotation = true;

		//camera ������Ʈ ���̱�
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

		// ���� ���Ϳ� �ӵ�, DeltaTime�� �̿��Ͽ� �̵� ���� ��� (X, Y, Z ��� ����)
		FVector Movement = direction * walkSpeed * DeltaTime;

		// ĳ������ ���� ��ġ�� ������
		FVector CurrentLocation = GetActorLocation();

		// ���� ��ġ�� ���
		FVector NewLocation = CurrentLocation + Movement;

		// ���� ��ġ�� ĳ���͸� �̵�
		SetActorLocation(NewLocation);

		// �̵� ���͸� �α׷� ����Ͽ� Ȯ��
		UE_LOG(LogTemp, Warning, TEXT("Movement -> X: %f, Y: %f, Z: %f"), Movement.X, Movement.Y, Movement.Z);

		// �� �����Ӹ��� direction �ʱ�ȭ
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
		// 3D ���ͷ� �����Ͽ� X, Y, Z �� ��� ó��
		FVector value = inputValue.Get<FVector>();

		// X, Y �� ���� ó��
		direction.X = value.X;
		direction.Y = value.Y;
		// Z �� ���� (Fly �Է¿� ���� ��/�Ʒ� �̵�)
		direction.Z = value.Z; // Z ���� Fly���� ������ ��
	}
	void AC_Player::InputJump(const FInputActionValue& inputValue)
	{

		Jump();
	}
	void AC_Player::PlayerMove()
	{
		// ��ü ���� ���͸� �� ���� ó�� (X, Y, Z ��� ����)
		FVector FinalDirection = FVector(direction.X, direction.Y, direction.Z);

		// ī�޶��� ȸ������ ����Ͽ� ���͸� ��ȯ
		FVector TransformedDirection = FTransform(GetControlRotation()).TransformVector(FinalDirection);

		// ���� �̵� ó��
		AddMovementInput(TransformedDirection);

		// �̵� ���͸� �α׷� ���
		UE_LOG(LogTemp, Warning, TEXT("TransformedDirection -> X: %f, Y: %f, Z: %f"), TransformedDirection.X, TransformedDirection.Y, TransformedDirection.Z);

		// �� �����Ӹ��� direction �ʱ�ȭ
		direction = FVector::ZeroVector;
	}


	void AC_Player::Fly(const FInputActionValue& inputValue)
	{
		// Z�� ���� �̵� ���� ���� (���: ����, ����: �Ʒ���)
		float flyValue = inputValue.Get<float>();

		// Z�� �̵� �ӵ��� ����Ͽ� �� ����
		direction.Z = flyValue;

		// �Է� ���� ���� Z�� �� �α� ���
		//UE_LOG(LogTemp, Warning, TEXT("Input Value: %f, Calculated Z: %f"), flyValue, direction.Z);
	}

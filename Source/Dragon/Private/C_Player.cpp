	#include "C_Player.h"
	#include "C_WeaponComponent.h"
	#include <GameFramework/SpringArmComponent.h>
	#include <Camera/CameraComponent.h>
	#include "EnhancedInputSubsystems.h"
	#include "EnhancedInputComponent.h"
	#include "InputActionValue.h"
    #include "GameFramework/CharacterMovementComponent.h"
	#include "GameFramework/Character.h"

	AC_Player::AC_Player()
	{
		PrimaryActorTick.bCanEverTick = true;

		WeaponComp = CreateDefaultSubobject<UC_WeaponComponent>(TEXT("WeaponComp"));

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
		springArmComp->TargetArmLength = 150;
		springArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
		springArmComp->SocketOffset = FVector(0.0f, 60.0f, 0.0f);
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
		MaxHP = 100;
		CurrentHP = MaxHP;
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

			
			OnInputBindingDelegate.Broadcast(PlayerInput);
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

		// �� �����Ӹ��� direction �ʱ�ȭ
		direction = FVector::ZeroVector;
	}


	void AC_Player::Fly(const FInputActionValue& inputValue)
	{
		// Z�� ���� �Է� �� (����� ��� ����, ������ ��� �Ʒ���)
		float flyValue = inputValue.Get<float>();
		

		// flyValue�� ����̰�, ���� ���� �ƴϸ� ���� ����
		if (flyValue > 0.0f && bCanFly)
		{
			direction.Z = flyValue;
			// �÷��̾ ���� ����
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->GravityScale = 0.0f;
			GetCharacterMovement()->BrakingDecelerationFlying = 2048.0f;

			// �̹� ���� ���� �ƴϸ� Ÿ�̸� ����
			if (!bIsFlying)
			{
				GetWorldTimerManager().SetTimer(FlightTimerHandle, this, &AC_Player::StopFlying, FlightTime, false);
				UE_LOG(LogTemp, Warning, TEXT("Timer set! FlightTime: %f seconds"), FlightTime);
			}

			// ���� ���¸� true�� ����
			bIsFlying = true;
		}
		else if (flyValue < 0.0f)
		{
			// ���� �� �Է� �� ��� ���� �ߴ�
			StopFlying();
		}
	}
	void AC_Player::StopFlying()
	{
		// ���� ��带 �ߴ��ϰ� �ٽ� �ȱ� ���� ��ȯ
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;

		// ���� ���¸� ����
		bIsFlying = false;

		// ������ ����Ǹ� �ٷ� ������ ���ϰ� ����
		bCanFly = false;  // ���� �Ұ��� ���·� ����

		// Ÿ�̸� �ʱ�ȭ
		GetWorldTimerManager().ClearTimer(FlightTimerHandle);

		// ��Ÿ�� �� �ٽ� ������ �� �ֵ��� Ÿ�̸� ����
		if (!GetWorldTimerManager().IsTimerActive(CooldownTimerHandle)) // �ߺ� ����
		{
			GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AC_Player::ResetFlyAbility, CooldownTime, false);
		}

		// �α� ���
		UE_LOG(LogTemp, Warning, TEXT("StopFlying called, bCanFly set to false. Cooldown started."));
	}


	void AC_Player::ResetFlyAbility()
	{
		// ��Ÿ�� �� �ٽ� �� �� �ְ� ����
		bCanFly = true;
	}

	float AC_Player::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Player Damaged"));
		CurrentHP -= DamageAmount;
		if (CurrentHP <= 0)
		{
			// �÷��̾ �׾��� �� ó��
			// ���� ���� ó�� ��
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Player Dead"));
		}
		return 0.0f;
	}

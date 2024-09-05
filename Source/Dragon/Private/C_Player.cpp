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
		springArmComp->TargetArmLength = 150;
		springArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
		springArmComp->SocketOffset = FVector(0.0f, 60.0f, 0.0f);
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

		// 매 프레임마다 direction 초기화
		direction = FVector::ZeroVector;
	}


	void AC_Player::Fly(const FInputActionValue& inputValue)
	{
		// Z축 방향 입력 값 (양수일 경우 위로, 음수일 경우 아래로)
		float flyValue = inputValue.Get<float>();
		

		// flyValue가 양수이고, 비행 중이 아니면 비행 시작
		if (flyValue > 0.0f && bCanFly)
		{
			direction.Z = flyValue;
			// 플레이어가 날기 시작
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->GravityScale = 0.0f;
			GetCharacterMovement()->BrakingDecelerationFlying = 2048.0f;

			// 이미 비행 중이 아니면 타이머 설정
			if (!bIsFlying)
			{
				GetWorldTimerManager().SetTimer(FlightTimerHandle, this, &AC_Player::StopFlying, FlightTime, false);
				UE_LOG(LogTemp, Warning, TEXT("Timer set! FlightTime: %f seconds"), FlightTime);
			}

			// 비행 상태를 true로 설정
			bIsFlying = true;
		}
		else if (flyValue < 0.0f)
		{
			// 음수 값 입력 시 즉시 날기 중단
			StopFlying();
		}
	}
	void AC_Player::StopFlying()
	{
		// 비행 모드를 중단하고 다시 걷기 모드로 전환
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;

		// 비행 상태를 종료
		bIsFlying = false;

		// 비행이 종료되면 바로 비행을 못하게 설정
		bCanFly = false;  // 비행 불가능 상태로 변경

		// 타이머 초기화
		GetWorldTimerManager().ClearTimer(FlightTimerHandle);

		// 쿨타임 후 다시 비행할 수 있도록 타이머 설정
		if (!GetWorldTimerManager().IsTimerActive(CooldownTimerHandle)) // 중복 방지
		{
			GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AC_Player::ResetFlyAbility, CooldownTime, false);
		}

		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("StopFlying called, bCanFly set to false. Cooldown started."));
	}


	void AC_Player::ResetFlyAbility()
	{
		// 쿨타임 후 다시 날 수 있게 설정
		bCanFly = true;
	}

	float AC_Player::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Player Damaged"));
		CurrentHP -= DamageAmount;
		if (CurrentHP <= 0)
		{
			// 플레이어가 죽었을 때 처리
			// 게임 오버 처리 등
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Player Dead"));
		}
		return 0.0f;
	}

// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Enemy.h"

// Sets default values
AC_Enemy::AC_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Project/Asset/FourEvilDragonsPBR/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'"));
	if (EnemyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(EnemyMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> EnemyAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Boss/B_Blueprints/ABP_EnemyAnim.ABP_EnemyAnim_C'"));
	if (EnemyAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(EnemyAnim.Class);
	}
	EnemyFSM = CreateDefaultSubobject<UC_EnemyFSM>(TEXT("EnemyFSM"));
}

// Called when the game starts or when spawned
void AC_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AC_Enemy::Die()
{
}

float AC_Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHP <= 0)
	{
		Die();
	}
	EnemyFSM->OnDamageProcess();
	return 0.0f;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Dragon/DragonCharacter.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	target = Cast<ADragonCharacter>(actor);
	if (target == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, "Target is null");
	}
	me = Cast<AEnemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logMsg);

	switch (mState) 
	{
		case EEnemyState::Idle:
			IdleState();
			break;
		case EEnemyState::Move:
			MoveState();
			break;
		case EEnemyState::Attack:
			AttackState();
			break;
		case EEnemyState::Damage:
			DamageState();
			break;
		case EEnemyState::Die:
			DieState();
			break;
	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime >= idleDelayTime)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	//UE_LOG(Warning, TEXT("Destination: %s"), *(destination.ToString().GetCharArray()));
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	//if (dir.Size() < attackRange)
	//{
	//	mState = EEnemyState::Attack;
	//}
}

void UEnemyFSM::AttackState()
{
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}


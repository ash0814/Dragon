// Fill out your copyright notice in the Description page of Project Settings.


#include "C_EnemyFSM.h"
#include "C_Enemy.h"
#include "C_Player.h"
#include "C_EnemyAnimInstance.h"
#include "C_GameMode.h"
#include "C_MainUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include "Navigation/PathFollowingComponent.h"


// Sets default values for this component's properties
UC_EnemyFSM::UC_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UC_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AC_Player::StaticClass());
	player = Cast<AC_Player>(actor);
	me = Cast<AC_Enemy>(GetOwner());

	anim = Cast<UC_EnemyAnimInstance>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());
	
}


// Called every frame
void UC_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	
	switch(mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Fly:
		FlyState();
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

void UC_EnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{
		SetState(EEnemyState::Move);
		currentTime = 0;
		//GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UC_EnemyFSM::MoveState()
{
	FVector destination = player->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	
	SetLocationToPlayer();
	
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;	// struct
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	ai->BuildPathfindingQuery(req, query);
	FPathFindingResult r = ns->FindPathSync(query);
	
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);

		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	if (dir.Size() < attackRange)
	{
		ai->StopMovement();
		SetState(EEnemyState::Attack);
		anim->bAttackPlay = true;
		currentTime = 0;
	} else if (dir.Size() > 300)
	{
		SetState(EEnemyState::Fly);
		currentTime = 0;
	}
}

void UC_EnemyFSM::FlyState()
{
	bCanChangeState = false;
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= 10.0)
	{
		bCanChangeState = true;
		currentTime = 0;
		SetState(EEnemyState::Move);
	} else if (currentTime >= 7.0) {
		anim->bCanFlyForward = false;
	} else if (currentTime >= 3.0)
	{
		SetLocationToPlayer();
		anim->bCanFlyForward = true;
	}
}

void UC_EnemyFSM::AttackState()
{
	bCanChangeState = false;
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= 3.0)
	{
		bCanChangeState = true;
		currentTime = 0;
		SetState(EEnemyState::Move);
	}
}

void UC_EnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		SetState(EEnemyState::Idle);
		currentTime = 0;
		SetState(EEnemyState::Fly);
	}
}

void UC_EnemyFSM::DieState()
{
	bCanChangeState = false;
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > 2.2)
	{
		FVector P0 = me->GetActorLocation();
		FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		me->SetActorLocation(P);

		if (P.Z < -200.0f)
		{
			me->Destroy();
		}
	}
}

void UC_EnemyFSM::OnDamageProcess()
{
	if (mState == EEnemyState::Die)
	{
		return;
	}
	if (mState == EEnemyState::Fly) {
		SetLocationToPlayer();
		if (player->bIsFlying == false) {
			bCanChangeState = true;
			SetState(EEnemyState::Move);
		}
	}
	if (me->CurrentHP > 0)
	{
		SetState(EEnemyState::Damage);
		currentTime = 0;
	}
	else
	{
		auto GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode != nullptr)
			GameMode->GameClear();
		
		bCanChangeState = true;
		currentTime = 0;
		SetState(EEnemyState::Die);
		//anim->PlayDamageAnim(TEXT("Die"));
	}
	ai->StopMovement();
}

bool UC_EnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UC_EnemyFSM::SetState(EEnemyState newState)
{
	if (bCanChangeState == false)
	{
		return;
	}
	mState = newState;
	anim->animState = mState;
	bCanChangeState = true;
}

void UC_EnemyFSM::SetLocationToPlayer()
{
	FVector destination = player->GetActorLocation();
	ai->MoveToLocation(destination);
}


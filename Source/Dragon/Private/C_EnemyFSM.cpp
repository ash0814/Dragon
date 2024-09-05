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
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
	
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
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UC_EnemyFSM::MoveState()
{
	FVector destination = player->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal());
	
	//ai->MoveToLocation(destination);
	
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
		FVector destination = player->GetActorLocation();
		FVector dir = destination - me->GetActorLocation();	
		//me->AddMovementInput(dir.GetSafeNormal());
		ai->MoveToLocation(destination);
		anim->bCanFlyForward = true;
	}
}

void UC_EnemyFSM::AttackState()
{
	//currentTime += GetWorld()->DeltaTimeSeconds;	
	//if (currentTime > attackDelayTime)
	//{
	//	currentTime = 0;
	//	anim->bAttackPlay = true;
	//}
	bCanChangeState = false;
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= 3.0)
	{
		bCanChangeState = true;
		currentTime = 0;
		SetState(EEnemyState::Fly);
	}
	/*FVector distance = player->GetActorLocation() - me->GetActorLocation();
	if (distance.Size() > attackRange)
	{
		SetState(EEnemyState::Move);
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}*/
}

void UC_EnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		SetState(EEnemyState::Idle);
		currentTime = 0;
	}
}

void UC_EnemyFSM::DieState()
{
	if (anim->bDieDone == false)
	{
		return;
	}

	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds; 
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
}

void UC_EnemyFSM::OnDamageProcess()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("OnDamageProcess"));
	if (mState == EEnemyState::Die)
	{
		return;
	}
	if (me->CurrentHP > 0)
	{
		SetState(EEnemyState::Damage);

		currentTime = 0;

		//int32 index = FMath::RandRange(0, 1);
		//FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		//anim->PlayDamageAnim(*sectionName);
	}
	else
	{
		// Get Gamemode
		auto GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode != nullptr)
			GameMode->GameOver();
		SetState(EEnemyState::Die);
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		anim->PlayDamageAnim(TEXT("Die"));
	}
	//anim->animState = mState;
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
}

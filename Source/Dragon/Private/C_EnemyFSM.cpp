// Fill out your copyright notice in the Description page of Project Settings.


#include "C_EnemyFSM.h"
#include "C_Enemy.h"
#include "C_Player.h"
#include "C_EnemyAnimInstance.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>

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

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AC_Enemy::StaticClass());
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
		mState = EEnemyState::Move;
		currentTime = 0;
		anim->animState = mState;
	}
}

void UC_EnemyFSM::MoveState()
{
	FVector destination = player->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal());
	ai->MoveToLocation(destination);
	//auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//FPathFindingQuery query;
	//FAIMoveRequest req;	// struct
	//req.SetAcceptanceRadius(3);
	//req.SetGoalLocation(destination);
	//ai->BuildPathfindingQuery(req, query);
	//FPathFindingResult r = ns->FindPathSync(query);
	
	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attack;
		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UC_EnemyFSM::FlyState()
{
}

void UC_EnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		// PRINT_LOG(TEXT("Attack!!!!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}

	FVector distance = player->GetActorLocation() - me->GetActorLocation();
	if (distance.Size() > attackRange)
	{
		mState = EEnemyState::Move;
		anim->animState = mState;
	}
}

void UC_EnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
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
	hp--;
	if (hp>0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;

		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(*sectionName);
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = mState;
}

bool UC_EnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}
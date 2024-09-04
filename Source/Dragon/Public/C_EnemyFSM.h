#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EnemyFSM.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Fly,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGON_API UC_EnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_EnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
    EEnemyState mState = EEnemyState::Idle;

    void IdleState();
    void MoveState();
    void FlyState();
    void AttackState();
    void DamageState();
    void DieState();

    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float idleDelayTime = 2;
    float currentTime = 0;

    UPROPERTY(VisibleAnywhere, Category = FSM)
    class AC_Player* player;

    UPROPERTY()
    class AC_Enemy* me;

    UPROPERTY(EditAnywhere, Category = FSM)
    float attackRange = 150.0f;

    UPROPERTY(EditAnywhere, Category = FSM)
    float attackDelayTime = 2.0f;

    void OnDamageProcess();        // event function

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    int32 hp = 30;

    UPROPERTY(EditAnywhere, Category = FSM)
    float damageDelayTime = 2.0f;

    UPROPERTY(EditAnywhere, Category = FSM)
    float dieSpeed = 50.0f;

    UPROPERTY()
    class UC_EnemyAnimInstance* anim;

    UPROPERTY()
    class AAIController *ai;

    FVector randomPos;
    bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
};

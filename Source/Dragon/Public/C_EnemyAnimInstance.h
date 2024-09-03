#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_EnemyFSM.h"
#include "C_EnemyAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class DRAGON_API UC_EnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
    EEnemyState animState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
    bool bAttackPlay = false;

    UFUNCTION(BlueprintCallable, Category = FSMEvent)
    void OnEndAttackAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
    void PlayDamageAnim(FName sectionName);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    bool bDieDone = false;
};

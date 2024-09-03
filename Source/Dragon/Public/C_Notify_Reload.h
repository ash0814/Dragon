#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_Notify_Reload.generated.h"

UENUM()
enum class EReloadType : uint8
{
	Eject, Spawn, Load, End,
};

UCLASS()
class DRAGON_API UC_Notify_Reload : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	EReloadType ActionType;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};

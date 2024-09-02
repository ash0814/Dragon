#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AK47, MAX,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAGON_API UC_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::MAX; }
	FORCEINLINE bool IsAK47Mode() { return Type == EWeaponType::AK47; }


public:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TSubclassOf<class AC_Weapon>> WeaponClasses;


public:	
	UC_WeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AC_Weapon* GetCurrentWeapon();

public://Set Current Mode
	void SetUnarmedMode();
	void SetAK47Mode();

private://Set Mode & Change Mode
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public://Notify Call
	void Begin_Equip();
	void End_Equip();

public:
	void Begin_Fire();
	void End_Fire();

public:
	FVector GetLefrHandLocation();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EWeaponType Type = EWeaponType::MAX;

private:
	TArray<class AC_Weapon*> Weapons;
	class ACharacter* OwnerCharacter;
		
};

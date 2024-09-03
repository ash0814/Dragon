#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "C_Weapon.generated.h"

UCLASS()
class DRAGON_API AC_Weapon : public AActor
{
	GENERATED_BODY()
	
public:
	//Animinstance에서 넘겨줄 IK Location
	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }

protected: //Equip Value
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;			//Weapon Base Attach Socket Name

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	class UAnimMontage* EquipMontage;	//Weapon Equip Animation

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float EquipMontage_PlayRate;		//Animation PlayRate

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightSocketName;				//Weapon Attach SocketName

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FVector LeftHandLocation;			//LeftHand IK Target Location
	
protected:	//Fire Value
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<class AC_Bullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	class UParticleSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	class UParticleSystem* EjectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	class USoundWave* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireInterval = 0.1f;	//Fire Speed
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilRate;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	int32 BulletCount;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	float ReloadMontage_PlayRate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	class UMaterialInstanceConstant* HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	class UParticleSystem* HitParticle;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

public:	
	AC_Weapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public://Equip Func
	bool CanEquip();
	void Equip();

	void Begin_Equip();
	void End_Equip();

	bool CanUnEquip();
	void UnEquip();

public://Fire Func
	bool CanFire();
	void Begin_Fire();
	void End_Fire();

	void OnFiring();

public://Reload
	void Reload();

protected:
	class ACharacter* OwnerCharacter;

private:
	bool bEquipping;
	bool bFiring;
	bool bReloading;

	FTimerHandle FireHandle;
};

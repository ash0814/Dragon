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
	
public://Reload
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	float ReloadMontage_PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reload")
	int32 DefaultBulletCount;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	TSubclassOf<class AC_Magazine> MagazineClass;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	FName MagazineBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	FName MagazineSocketName;

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

protected: //Hit
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
	bool CanReload();
	void Reload();

	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();

protected:
	class ACharacter* OwnerCharacter;

private:
	bool bEquipping;
	bool bFiring;
	bool bReloading;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentBulletCount;
	class AC_Magazine* Magazine;

	FTimerHandle FireHandle;
};

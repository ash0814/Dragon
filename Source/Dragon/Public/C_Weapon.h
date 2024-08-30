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

protected:
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


public:
	bool CanEquip();
	void Equip();

	void Begin_Equip();
	void End_Equip();

	bool CanUnEquip();
	void UnEquip();

private:
	bool bEquipping;

protected:
	class ACharacter* OwnerCharacter;

};

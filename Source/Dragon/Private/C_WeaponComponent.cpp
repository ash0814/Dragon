#include "C_WeaponComponent.h"
#include "CHelpers.h"

#include "C_Weapon.h"
#include "C_Player.h"

#include "GameFramework/Character.h"

UC_WeaponComponent::UC_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UC_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNUll(OwnerCharacter);

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (TSubclassOf<AC_Weapon> weaponClass : WeaponClasses)
	{
		if (!!weaponClass)
		{
			AC_Weapon* weapon = OwnerCharacter->GetWorld()->SpawnActor<AC_Weapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
	}
}


void UC_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AC_Weapon* UC_WeaponComponent::GetCurrentWeapon()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);

	return Weapons[(int32)Type];
}

void UC_WeaponComponent::SetUnarmedMode()
{
	CheckFalse(GetCurrentWeapon()->CanUnEquip());

	GetCurrentWeapon()->UnEquip();
	ChangeType(EWeaponType::MAX);
}

void UC_WeaponComponent::SetAK47Mode()
{
	SetMode(EWeaponType::AK47);
}

void UC_WeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		CheckFalse(GetCurrentWeapon()->CanUnEquip());
		GetCurrentWeapon()->UnEquip();
	}

	CheckNUll(Weapons[(int32)InType]);
	CheckFalse(Weapons[(int32)InType]->CanEquip());

	//Weapon의 장착명령 call
	Weapons[(int32)InType]->Equip();

	ChangeType(InType);
}

void UC_WeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
	{
		OnWeaponTypeChanged.Broadcast(type, InType);
	}
}

void UC_WeaponComponent::Begin_Equip()
{
	CheckNUll(GetCurrentWeapon());

	//C_Weapon의 Begin_Equip Call
	GetCurrentWeapon()->Begin_Equip();
}

void UC_WeaponComponent::End_Equip()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->End_Equip();
}

void UC_WeaponComponent::Begin_Fire()
{
	CheckNUll(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanFire());

	GetCurrentWeapon()->Begin_Fire();
}

void UC_WeaponComponent::End_Fire()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->End_Fire();
}

#include "C_Weapon.h"
#include "CHelpers.h"

#include "GameFramework/Character.h"

#include "C_WeaponComponent.h"

#include "Components/SkeletalMeshComponent.h"


AC_Weapon::AC_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AC_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (HolsterSocketName.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
	}
}

void AC_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AC_Weapon::CanEquip()
{
	bool b = false;
	b |= bEquipping;

	return !b;
}

void AC_Weapon::Equip()
{
	bEquipping = true;

	if (!!EquipMontage)
		OwnerCharacter->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);
}

void AC_Weapon::Begin_Equip()
{
	if (RightSocketName.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightSocketName);
	}
}

void AC_Weapon::End_Equip()
{
	bEquipping = false;
}

bool AC_Weapon::CanUnEquip()
{
	bool b = false;

	b |= bEquipping;

	return !b;
}

void AC_Weapon::UnEquip()
{
	if (HolsterSocketName.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
	}
}


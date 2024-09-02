#include "C_WeaponComponent.h"
#include "CHelpers.h"

#include "C_Weapon.h"
#include "C_Player.h"

#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/CanvasPanelSlot.h"

UC_WeaponComponent::UC_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UC_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	////SetMode Delegate Bind
	//OnSetMode.AddDynamic(this, &UC_WeaponComponent::SetAK47Mode);
	////////////////////////////////////////////////////////////////

	////Fire Delegate Bind
	//OnBegin_Fire.AddDynamic(this, &UC_WeaponComponent::Begin_Fire);
	//OnEnd_Fire.AddDynamic(this, &UC_WeaponComponent::End_Fire);
	////////////////////////////////////////////////////////////////

	if (CrossHairClass)
	{
		CrossHair = CreateWidget<UUserWidget>(GetWorld(), CrossHairClass);

		if (CrossHair)
		{
			CrossHair->AddToViewport();
			CrossHair->SetVisibility(ESlateVisibility::Hidden);
		}
	}


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

	if (CrossHair && CrossHair->IsVisible())
	{
		UpdateCrossHair();
	}
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
	HideCrossHair();
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

	//Show UI
	ShowCrossHair();

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
	//CheckTrue(OwnerCharacter->GetVelocity().Size2D() > 300)

	GetCurrentWeapon()->Begin_Fire();
}

void UC_WeaponComponent::End_Fire()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->End_Fire();
}

FVector UC_WeaponComponent::GetLefrHandLocation()
{
	CheckNullResult(GetCurrentWeapon(), FVector::ZeroVector);

	return GetCurrentWeapon()->GetLeftHandLocation();
}

void UC_WeaponComponent::ShowCrossHair()
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(ESlateVisibility::Visible);
	}
}

void UC_WeaponComponent::HideCrossHair()
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UC_WeaponComponent::UpdateCrossHair()
{
	if (!CrossHair)
		return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	int32 X, Y;
	PlayerController->GetViewportSize(X, Y);

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CrossHair->Slot);

	if (PanelSlot)
	{
		PanelSlot->SetSize(FVector2D(50.0f, 50.0f));
		PanelSlot->SetPosition(FVector2D(X / 2 - 25, Y / 2 - 25));
	}
}

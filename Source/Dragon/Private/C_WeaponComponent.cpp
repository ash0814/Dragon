#include "C_WeaponComponent.h"
#include "CHelpers.h"

#include "C_Weapon.h"
#include "C_Player.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/CanvasPanelSlot.h"

UC_WeaponComponent::UC_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
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

	if (CrossHairClass)
	{
		CrossHair = CreateWidget<UUserWidget>(GetWorld(), CrossHairClass);

		if (CrossHair)
		{
			CrossHair->AddToViewport();
			CrossHair->SetVisibility(ESlateVisibility::Hidden);
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

void UC_WeaponComponent::SetupInputBinding(UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(IA_Equip, ETriggerEvent::Started, this, &UC_WeaponComponent::SetAK47Mode);

	PlayerInput->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &UC_WeaponComponent::Begin_Fire);
	PlayerInput->BindAction(IA_Fire, ETriggerEvent::Completed, this, &UC_WeaponComponent::End_Fire);

	PlayerInput->BindAction(IA_Reload, ETriggerEvent::Started, this, &UC_WeaponComponent::Reload);
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

	//Weapon�� �������� call
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

	//C_Weapon�� Begin_Equip Call
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

void UC_WeaponComponent::Reload()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->Reload();
}

void UC_WeaponComponent::Eject_Magazine()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->Eject_Magazine();
}

void UC_WeaponComponent::Spawn_Magazine()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->Spawn_Magazine();
}

void UC_WeaponComponent::Load_Magazine()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->Load_Magazine();
}

void UC_WeaponComponent::End_Reload()
{
	CheckNUll(GetCurrentWeapon());

	GetCurrentWeapon()->End_Reload();
}

//Delegate Init Function
void UC_WeaponComponent::InitializeComponent()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (AC_Player* player = Cast<AC_Player>(OwnerCharacter))
	{
		player->OnInputBindingDelegate.AddUObject(this, &UC_WeaponComponent::SetupInputBinding);
	}
}
///////////////////////////////////////////////////////////////

FVector UC_WeaponComponent::GetLefrHandLocation()
{
	CheckNullResult(GetCurrentWeapon(), FVector::ZeroVector);

	return GetCurrentWeapon()->GetLeftHandLocation();
}


//CrossHair UI Function
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
///////////////////////////////////////////////////////////////

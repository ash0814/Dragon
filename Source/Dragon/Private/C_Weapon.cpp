#include "C_Weapon.h"
#include "CHelpers.h"

#include "C_WeaponComponent.h"
#include "C_Bullet.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"


AC_Weapon::AC_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	//BulletClass DefaultSetting
	{
		static ConstructorHelpers::FClassFinder<AC_Bullet> bullet(TEXT("/Script/Engine.Blueprint'/Game/Player/P_BulePrint/BP_C_Bullet.BP_C_Bullet_C'"));
		if (bullet.Succeeded())
			BulletClass = bullet.Class;
	}

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
	b |= bFiring;

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
	b |= bFiring;

	return !b;
}

void AC_Weapon::UnEquip()
{
	if (HolsterSocketName.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
	}
}

bool AC_Weapon::CanFire()
{
	bool b = false;

	b |= bEquipping;
	b |= bFiring;

	return !b;
}

void AC_Weapon::Begin_Fire()
{
	bFiring = true;

	//FireInterval 마다 OnFiring() 호출
	GetWorld()->GetTimerManager().SetTimer(FireHandle, this, &AC_Weapon::OnFiring, FireInterval, true, 0);

	OnFiring();
}

void AC_Weapon::End_Fire()
{
	CheckFalse(bFiring);

	if (GetWorld()->GetTimerManager().IsTimerActive(FireHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FireHandle);
	}

	bFiring = false;
}

void AC_Weapon::OnFiring()
{
	UCameraComponent* camera = Cast<UCameraComponent>(Owner->GetComponentByClass(UCameraComponent::StaticClass()));

	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	//LineTrace Start & End Locaiton
	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAngle);

	FVector end = transform.GetLocation() + direction * HitDistance;
	///////////////////////////////////////////////////

	//LineTrace 
	TArray<AActor*> ignores;
	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	//Recoil
	OwnerCharacter->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.5f, 1.2f));
	/////////////////////////////////////////////////////

	if (!!BulletClass)
	{
		FVector location = Mesh->GetSocketLocation("Muzzle_Bullet");

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AC_Bullet* bullet = GetWorld()->SpawnActor<AC_Bullet>(BulletClass, location, direction.Rotation(), params);

		if (!!bullet)
		{
			bullet->Shoot(direction);
		}
	}
}


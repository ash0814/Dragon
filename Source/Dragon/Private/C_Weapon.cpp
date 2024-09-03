#include "C_Weapon.h"
#include "CHelpers.h"

#include "C_WeaponComponent.h"
#include "C_Bullet.h"

#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstanceConstant.h"


AC_Weapon::AC_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	//BulletClass DefaultSetting
	{
		//Bullet Class
		static ConstructorHelpers::FClassFinder<AC_Bullet> bullet(TEXT("/Script/Engine.Blueprint'/Game/Player/P_BulePrint/BP_C_Bullet.BP_C_Bullet_C'"));
		if (bullet.Succeeded())
			BulletClass = bullet.Class;

		//HitDecal
		static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> Decal(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Project/Asset/Materials/Texture/Bullet_Hole_Mat_Inst.Bullet_Hole_Mat_Inst'"));
		if (Decal.Succeeded())
			HitDecal = Decal.Object;

		//Muzzle 
		static ConstructorHelpers::FObjectFinder<UParticleSystem> Muzzle(TEXT("/Script/Engine.ParticleSystem'/Game/Project/Asset/Effects/MilitaryWeapSilver/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
		if (Muzzle.Succeeded())
			FlashParticle = Muzzle.Object;

		//Fire Sound
		static ConstructorHelpers::FObjectFinder<USoundWave> Sound(TEXT("/Script/Engine.SoundWave'/Game/Project/Asset/Gun_Explosion_Sounds/Gun_01/wav/Gun_1_1.Gun_1_1'"));
		if (Sound.Succeeded())
			FireSound = Sound.Object;
		
		//HitParticle
		static ConstructorHelpers::FObjectFinder<UParticleSystem> Hit(TEXT("/Script/Engine.ParticleSystem'/Game/Project/Asset/Effects/BulletEffects/P_Impact_Default.P_Impact_Default'"));
		if (Hit.Succeeded())
			HitParticle = Hit.Object;
		
		//Eject
		static ConstructorHelpers::FObjectFinder<UParticleSystem> Eject(TEXT("/Script/Engine.ParticleSystem'/Game/Project/Asset/Effects/BulletEffects/P_Eject_bullet.P_Eject_bullet'"));
		if (Eject.Succeeded())
			EjectParticle = Eject.Object;
	
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
	FVector end = transform.GetLocation() + direction * HitDistance;
	///////////////////////////////////////////////////

	//LineTrace 
	TArray<AActor*> ignores;
	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	if (BulletCount > 0)
	{
		//Recoil
		OwnerCharacter->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.5f, 1.2f));
		/////////////////////////////////////////////////////

		//Weapon Spread
		direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAngle);
		/////////////////////////////////////////////////////

		if (hitResult.bBlockingHit)
		{
			if (!!HitDecal)
			{
				FRotator rotator = hitResult.ImpactNormal.Rotation();

				UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), hitResult.Location, rotator, 10);
				decal->SetFadeScreenSize(0);
			}

			if (!!HitParticle)
			{
				FRotator rotator = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, rotator);
			}
		}

		if (!!FlashParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
		}

		if (!!EjectParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "Eject", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
		}

		FVector MuzzleLocation = Mesh->GetSocketLocation("Muzzle");

		if (!!FireSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzleLocation);
		}

		if (!!BulletClass)
		{

			FVector location = Mesh->GetSocketLocation("Muzzle_Bullet");

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AC_Bullet* bullet = GetWorld()->SpawnActor<AC_Bullet>(BulletClass, location, direction.Rotation(), params);

			if (!!bullet)
			{
				bullet->Shoot(direction);
				BulletCount--;
			}

		}
	}

}

void AC_Weapon::Reload()
{
	bReloading = true;

	if (!!ReloadMontage)
		OwnerCharacter->PlayAnimMontage(ReloadMontage, ReloadMontage_PlayRate);
}


#include "C_Bullet.h"
#include "CHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"

AC_Bullet::AC_Bullet()
{
	//Create Component
	{
		Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

		SetRootComponent(Capsule);
		Mesh->SetupAttachment(Capsule);
	}

	//Set Object
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		if (StaticMesh.Succeeded())
			Mesh->SetStaticMesh(StaticMesh.Object);

		static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> material(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Player/P_BulePrint/Materials/M_Bullet_Inst.M_Bullet_Inst'"));
		if (material.Succeeded())
			Mesh->SetMaterial(0, material.Object);
	}

	//Setting
	{
		Capsule->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		Capsule->SetCapsuleHalfHeight(50);
		Capsule->SetCapsuleRadius(2);
		Capsule->SetCollisionProfileName("BlockAllDynamic");

		Mesh->SetRelativeScale3D(FVector(1.0f, 0.025f, 0.025f));
		Mesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

		Projectile->InitialSpeed = 2e+4f;
		Projectile->MaxSpeed = 2e+4f;
		Projectile->ProjectileGravityScale = 0;

		BulletDamage = 10.0f;
	}
}

void AC_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
	Projectile->SetActive(false);
	Capsule->OnComponentHit.AddDynamic(this, &AC_Bullet::OnHit);
}

void AC_Bullet::Shoot(const FVector& InDir)
{
	SetLifeSpan(3);

	Projectile->Velocity = InDir * Projectile->InitialSpeed;
	Projectile->SetActive(true);
}

void AC_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	Destroy();
}


#include "C_Magazine.h"

#include "Components/StaticMeshComponent.h"

AC_Magazine::AC_Magazine()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh_Full = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_Full"));
	Mesh_Empty = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_Empty"));
	Mesh_Full->SetupAttachment(Root);
	Mesh_Empty->SetupAttachment(Root);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Full(TEXT("/Script/Engine.StaticMesh'/Game/Project/Asset/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SM_KA47_Mag.SM_KA47_Mag'"));
	if (Full.Succeeded())
		Mesh_Full->SetStaticMesh(Full.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Empty(TEXT("/Script/Engine.StaticMesh'/Game/Project/Asset/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SM_KA47_Mag_Empty.SM_KA47_Mag_Empty'"));
	if (Empty.Succeeded())
		Mesh_Empty->SetStaticMesh(Empty.Object);

	Mesh_Full->SetCollisionProfileName("PhysicsActor");
	Mesh_Empty->SetCollisionProfileName("PhysicsActor");
}

void AC_Magazine::BeginPlay()
{
	Super::BeginPlay();

	if (bEject == false)
		Mesh_Full->SetVisibility(true);
}

void AC_Magazine::SetEject()
{
	bEject = true;

	Mesh_Full->SetVisibility(false);
	Mesh_Empty->SetVisibility(true);
}


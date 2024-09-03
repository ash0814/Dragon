// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Crystal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AC_Crystal::AC_Crystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TotalHP = 100;
	CurrentHP = 100;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(sphereComp);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComponent->SetupAttachment(sphereComp);

	float sphereSize = 50.0f;
	sphereComp->SetSphereRadius(sphereSize);
}

// Called when the game starts or when spawned
void AC_Crystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Crystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_Crystal::Die()
{
	Destroy();
}

float AC_Crystal::TakeDamage(float damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 데미지 처리
	float ActualDamage = Super::TakeDamage(damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		// 체력 감소
		CurrentHP -= ActualDamage;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurrentHP : %f"), CurrentHP));

		// 체력이 0 이하이면 사망 처리
		if (CurrentHP <= 0.0f)
		{
			Die();
		}
	}

	return ActualDamage;
}


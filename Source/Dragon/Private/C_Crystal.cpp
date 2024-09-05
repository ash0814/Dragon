#include "C_Crystal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "C_GameMode.h"
#include "C_MainUI.h"

// Sets default values
AC_Crystal::AC_Crystal()
{
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

void AC_Crystal::BeginPlay()
{
    Super::BeginPlay();

    // 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("AC_Crystal::BeginPlay Called"));
}

void AC_Crystal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AC_Crystal::Die()
{
    // GameMode에서 크리스탈 개수 감소
    AC_GameMode* GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode != nullptr)
    {
        GameMode->TotalCrystalCount--;
        UE_LOG(LogTemp, Warning, TEXT("Crystal Destroyed. Remaining Crystals: %d"), GameMode->TotalCrystalCount);

        // 모든 크리스탈이 파괴되었을 때
        if (GameMode->TotalCrystalCount <= 0)
        {
            GameMode->MainUI->OnChangeCrystalCountZero();
        }
    }
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

        UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f"), CurrentHP);

        // 체력이 0 이하이면 사망 처리
        if (CurrentHP <= 0.0f)
        {
            Die();
        }
    }

    return ActualDamage;
}

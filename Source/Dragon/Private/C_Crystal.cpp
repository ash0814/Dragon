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

    // �α� �߰�
    UE_LOG(LogTemp, Warning, TEXT("AC_Crystal::BeginPlay Called"));
}

void AC_Crystal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AC_Crystal::Die()
{
    // GameMode���� ũ����Ż ���� ����
    AC_GameMode* GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode != nullptr)
    {
        GameMode->TotalCrystalCount--;
        UE_LOG(LogTemp, Warning, TEXT("Crystal Destroyed. Remaining Crystals: %d"), GameMode->TotalCrystalCount);

        // ��� ũ����Ż�� �ı��Ǿ��� ��
        if (GameMode->TotalCrystalCount <= 0)
        {
            GameMode->MainUI->OnChangeCrystalCountZero();
        }
    }
    Destroy();
}

float AC_Crystal::TakeDamage(float damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // ������ ó��
    float ActualDamage = Super::TakeDamage(damage, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage > 0.0f)
    {
        // ü�� ����
        CurrentHP -= ActualDamage;

        UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f"), CurrentHP);

        // ü���� 0 �����̸� ��� ó��
        if (CurrentHP <= 0.0f)
        {
            Die();
        }
    }

    return ActualDamage;
}

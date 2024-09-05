#include "C_Enemy.h"
#include "C_Crystal.h"
#include "C_EnemyFSM.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "C_GameMode.h"

// Sets default values
AC_Enemy::AC_Enemy()
{
    PrimaryActorTick.bCanEverTick = true;

    ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Project/Asset/FourEvilDragonsPBR/Meshes/DragonTheUsurper/DragonTheUsurperSK.DragonTheUsurperSK'"));
    if (EnemyMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(EnemyMesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }

    ConstructorHelpers::FClassFinder<UAnimInstance> EnemyAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Boss/B_Blueprints/ABP_EnemyAnim.ABP_EnemyAnim_C'"));
    if (EnemyAnim.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(EnemyAnim.Class);
    }
    EnemyFSM = CreateDefaultSubobject<UC_EnemyFSM>(TEXT("EnemyFSM"));
}

void AC_Enemy::BeginPlay()
{
    Super::BeginPlay();

    CurrentHP = 100;
    MaxHP = 100;
}

void AC_Enemy::GetHurt(float Amount)
{
    EnemyFSM->OnDamageProcess();
    CurrentHP -= Amount;
    UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f"), CurrentHP);
}

float AC_Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!bCanHurt)
    {
        auto GameMode = Cast<AC_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode != nullptr && GameMode->TotalCrystalCount == 0)
        {
            bCanHurt = true;
            GetHurt(DamageAmount);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("YOU HAVE TO DESTROY ALL CRYSTALS FIRST"));
        }
    }
    else
    {
        GetHurt(DamageAmount);
    }
    return 0.0f;
}

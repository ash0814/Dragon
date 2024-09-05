#include "C_GameMode.h"
#include "C_MainUI.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "C_Crystal.h"
#include "C_Player.h"
#include "C_Weapon.h"
#include "C_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

AC_GameMode::AC_GameMode()
{
    // Set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/P_BulePrint/BP_CPlayer.BP_CPlayer_C'"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AC_GameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("AC_GameMode::BeginPlay Called"));

    CurrentState = EGameState::Ready;

    MainUI = CreateWidget<UC_MainUI>(GetWorld(), MainUIWidgetFactory);
    if (MainUI != nullptr)
        MainUI->AddToViewport();

    GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

    // 크리스탈 초기화 지연 설정 (0.2초 후 크리스탈 개수를 초기화)
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_GameMode::InitializeCrystalCount, 0.2f, false);
}

void AC_GameMode::InitializeCrystalCount()
{
    // 모든 크리스탈 액터를 검색하여 개수를 초기화
    TArray<AActor*> CrystalActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_Crystal::StaticClass(), CrystalActors);
    TotalCrystalCount = CrystalActors.Num();

    UE_LOG(LogTemp, Warning, TEXT("Total Crystals Found: %d"), TotalCrystalCount);
}

void AC_GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AC_GameMode::GameOver()
{
    MainUI->OnGameOver();

   
    // 3초 후에 레벨을 다시 로드
    FTimerHandle GameOverTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &AC_GameMode::LoadNextLevel, 3.0f, false);
}

void AC_GameMode::LoadNextLevel()
{
    // 현재 레벨 이름을 가져와서 다시 로드하는 코드
    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // 경로 부분 제거

    UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));

    // 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("Reloading Level: %s"), *CurrentLevelName);
}

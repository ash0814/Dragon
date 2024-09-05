#include "C_GameMode.h"
#include "C_MainUI.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "C_Crystal.h"
#include "C_Player.h"
#include "C_Weapon.h"
#include "C_WeaponComponent.h"
#include "C_PlayerUI.h"
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

	CurrentState = EGameState::Ready;
	MainUI = CreateWidget<UC_MainUI>(GetWorld(), MainUIWidgetFactory);
	if (MainUI != nullptr)
		MainUI->AddToViewport();
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

    // ũ����Ż �ʱ�ȭ ���� ���� (0.2�� �� ũ����Ż ������ �ʱ�ȭ)
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_GameMode::InitializeCrystalCount, 0.2f, false);
}

void AC_GameMode::InitializeCrystalCount()
{
    // ��� ũ����Ż ���͸� �˻��Ͽ� ������ �ʱ�ȭ
    TArray<AActor*> CrystalActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_Crystal::StaticClass(), CrystalActors);
    TotalCrystalCount = CrystalActors.Num();

    UE_LOG(LogTemp, Warning, TEXT("Total Crystals Found: %d"), TotalCrystalCount);
}

void AC_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EGameState::Ready:
		Ready();
		break;
	case EGameState::Start:
		Start();
		break;
	}
}



void AC_GameMode::Ready()
{
}

void AC_GameMode::Start()
{
}

void AC_GameMode::GameOver()
{
    MainUI->OnGameOver();

   
    // 3�� �Ŀ� ������ �ٽ� �ε�
    FTimerHandle GameOverTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &AC_GameMode::LoadNextLevel, 3.0f, false);
}

void AC_GameMode::LoadNextLevel()
{
    // ���� ���� �̸��� �����ͼ� �ٽ� �ε��ϴ� �ڵ�
    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // ��� �κ� ����

    UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));

    // �α� �߰�
    UE_LOG(LogTemp, Warning, TEXT("Reloading Level: %s"), *CurrentLevelName);
	// MainUI->OnGameOver();
	//// get player
	//AC_Player *player = Cast<AC_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//if (player != nullptr)
	//{
	//	// player weapon off
	//	player->WeaponComp->End_Fire();
	//}
	//GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	//GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

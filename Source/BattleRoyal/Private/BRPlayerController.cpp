#include "BRPlayerController.h"
#include "BRGameMode.h"
#include "Kismet/GameplayStatics.h"

void ABRPlayerController::ClientDeathResult_Implementation(int32 Rank)
{
    SetInputMode(UIMode);
    bShowMouseCursor = true;
    DeathResult(Rank);
}

void ABRPlayerController::ClientWinnerResult_Implementation()
{
    SetInputMode(UIMode);
    bShowMouseCursor = true;
    WinnerResult();
}

void ABRPlayerController::ClientGoToLobby_Implementation()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true);
}

void ABRPlayerController::Dead()
{
    ABRGameMode* BRGameMode = Cast<ABRGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (BRGameMode)
        BRGameMode->Dead(this);
}

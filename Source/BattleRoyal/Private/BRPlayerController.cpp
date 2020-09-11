#include "BRPlayerController.h"
#include "BRGameMode.h"

void ABRPlayerController::ClientDeathResult_Implementation(int32 Rank)
{
    DeathResult(Rank);
}

void ABRPlayerController::ClientWinnerResult_Implementation()
{
    WinnerResult();
}

void ABRPlayerController::Dead()
{
    ABRGameMode* BRGameMode = Cast<ABRGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (BRGameMode)
        BRGameMode->Dead(this);
}

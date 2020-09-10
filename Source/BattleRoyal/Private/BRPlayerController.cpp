#include "BRPlayerController.h"
#include "BRGameMode.h"

void ABRPlayerController::Dead()
{
    ABRGameMode* BRGameMode = Cast<ABRGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (BRGameMode)
        BRGameMode->Dead(this);
}

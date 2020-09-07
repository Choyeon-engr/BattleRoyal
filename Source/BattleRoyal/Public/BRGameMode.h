#pragma once

#include "BattleRoyal.h"
#include "GameFramework/GameModeBase.h"
#include "BRGameMode.generated.h"

UCLASS()
class BATTLEROYAL_API ABRGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
protected:
    void BeginPlay() override;
    
    APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString & Portal, const FString & Options, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;
    
private:
    FTimerHandle MainTimerHandle = { };
    
    enum class EGameProgress
    {
        READY = 0,
        BATTLE,
        RESULT
    } CurGameProgress;
};

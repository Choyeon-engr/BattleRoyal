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
    
    void PreLogin(const class FString & Options, const class FString & Address, const FUniqueNetIdRepl & UniqueId, class FString & ErrorMessage) override;
    
private:
    FTimerHandle MainTimerHandle = { };
    
    enum class EGameProgress
    {
        READY = 0,
        BATTLE,
        RESULT
    } CurGameProgress;
};

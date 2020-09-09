#pragma once

#include "BattleRoyal.h"
#include "GameFramework/GameModeBase.h"
#include "BRGameMode.generated.h"

UCLASS()
class BATTLEROYAL_API ABRGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    ABRGameMode();
    
protected:
    void BeginPlay() override;
    
    APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString & Portal, const FString & Options, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;
    
    void Logout(AController* Exiting) override;
    
private:
    void Broadcast(const FString & Message);
     
private:
    FTimerHandle MainTimerHandle = { };
    
    enum class EGameProgress
    {
        READY = 0,
        BATTLE,
        RESULT
    } CurGameProgress;
    
    UPROPERTY(Transient)
    TArray<class ABRPlayerController*> AliveClients;
    
    int32 MinNumOfPlayer;
    int32 TimeRemaining;
};

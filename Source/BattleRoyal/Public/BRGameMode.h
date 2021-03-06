#pragma once

#include "BattleRoyal.h"
#include "GameFramework/GameModeBase.h"
#include "BRGameMode.generated.h"

UCLASS()
class BATTLEROYAL_API ABRGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    ABRGameMode() : CurGameProgress(EGameProgress::READY), MinNumOfPlayer(2), ReadyTimeRemaining(10), ResultTimeRemaining(10), bOnBattleField(true) { }
    
    UFUNCTION(BlueprintImplementableEvent)
    void SpawnVehicle();
    
    void Dead(class ABRPlayerController* PlayerController);
    
    void Broadcast(const FString & Message);
    
protected:
    void BeginPlay() override;
    
    APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString & Portal, const FString & Options, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage) override;
    
    void Logout(AController* Exiting) override;
    
private:
    FTimerHandle MainTimerHandle = { };
    
    enum class EGameProgress
    {
        READY = 0,
        BATTLE,
        RESULT
    } CurGameProgress;
    
    UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TArray<class ABRPlayerController*> AliveClients;
    
    UPROPERTY(Transient)
    TArray<class ABRPlayerController*> DeadClients;
    
    UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    AActor* Vehicle;
    
    int32 MinNumOfPlayer;
    int32 ReadyTimeRemaining;
    int32 ResultTimeRemaining;
    
    bool bOnBattleField;
};

#pragma once

#include "BattleRoyal.h"
#include "GameFramework/PlayerController.h"
#include "BRPlayerController.generated.h"

UCLASS()
class BATTLEROYAL_API ABRPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintImplementableEvent)
    void MessageToClient(const FString & Message);
    
    UFUNCTION(Client, Reliable)
    void ClientDeathResult(int32 Rank);
    
    UFUNCTION(BlueprintImplementableEvent)
    void DeathResult(int32 Rank);
    
    UFUNCTION(Client, Reliable)
    void ClientWinnerResult();
    
    UFUNCTION(BlueprintImplementableEvent)
    void WinnerResult();
    
    UFUNCTION(Client, Reliable)
    void ClientGoToLobby();
    
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void BoardVehicle(AActor* Vehicle);
    
    void Dead();
    
protected:
    void BeginPlay() override;
    
private:
    FInputModeGameOnly GameMode;
    FInputModeUIOnly UIMode;
};

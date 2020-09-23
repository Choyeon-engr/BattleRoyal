#pragma once

#include "BattleRoyal.h"
#include "GameFramework/PlayerController.h"
#include "BRPlayerController.generated.h"

UCLASS()
class BATTLEROYAL_API ABRPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
    ABRPlayerController() : bDescent(false) { }
    
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
    
    UFUNCTION(BlueprintImplementableEvent)
    void BoardVehicle(AActor* Vehicle);
    
    UFUNCTION(Client, Unreliable)
    void ClientPrintBloodScreen();
    
    UFUNCTION(BlueprintImplementableEvent)
    void PrintBloodScreen();
    
    void Dead();
    
protected:
    void BeginPlay() override;
    
private:
    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    bool bDescent;
    
    FInputModeGameOnly GameMode;
    FInputModeUIOnly UIMode;
};

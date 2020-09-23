#include "BRGameMode.h"
#include "BRPlayerController.h"
#include "BRGameState.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "BRNamespace"

void ABRGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
        switch (CurGameProgress)
        {
            case EGameProgress::READY:
            {
                if (AliveClients.Num() >= MinNumOfPlayer)
                {
                    if (!ReadyTimeRemaining--)
                    {
                        CurGameProgress = EGameProgress::BATTLE;
                        Broadcast(FString(TEXT("Start the battle!")));
                        
                        ABRGameState* BRGameState = Cast<ABRGameState>(GetWorld()->GetGameState());
                        BRGameState->SetDamaged(true);
                        BRGameState->UpdateCircle();
                        
                        SpawnVehicle();
                        
                        for (int i = 0; i < AliveClients.Num(); ++i)
                            if (AliveClients[i] && Vehicle)
                                AliveClients[i]->BoardVehicle(Vehicle);
                        
                        FTimerHandle BroadcastTimerHandle = { };
                        GetWorld()->GetTimerManager().SetTimer(BroadcastTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
                            Broadcast(FString(TEXT("")));
                        }), 1.0f, false, 1.0f);
                    }
                    else
                        Broadcast(FText::Format(LOCTEXT("ReadyTimeRemaining", "{0} sec before the battle starts"), ReadyTimeRemaining).ToString());
                }
                else
                {
                    ReadyTimeRemaining = 10;
                    Broadcast(FString(TEXT("Waiting for another player!")));
                }
                
                break;
            }
            case EGameProgress::BATTLE:
            {
                ABRGameState* BRGameState = Cast<ABRGameState>(GetWorld()->GetGameState());
                BRGameState->UpdateCircle();
                BRGameState->SetSurvivor(AliveClients.Num());
                
                if (BRGameState->GetSurvivor() <= 1)
                {
                    AliveClients[0]->ClientWinnerResult();
                    CurGameProgress = EGameProgress::RESULT;
                }
                
                break;
            }
            case EGameProgress::RESULT:
            {
                if (!ResultTimeRemaining--)
                {
                    GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);
                    
                    for (int i = 0; i < AliveClients.Num(); ++i)
                        if (AliveClients[i])
                            AliveClients[i]->ClientGoToLobby();
                    
                    for (int j = 0; j < DeadClients.Num(); ++j)
                        if (DeadClients[j])
                            DeadClients[j]->ClientGoToLobby();
                    
                    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true, FString(TEXT("listen")));
                    
                    bOnBattleField = false;
                }
                else if (bOnBattleField)
                    Broadcast(FText::Format(LOCTEXT("ResultTimeRemaining", "{0} sec before moving to the lobby"), ResultTimeRemaining).ToString());
                break;
            }
        }
    }), 1.0f, true);
}

APlayerController* ABRGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString & Portal, const FString & Options, const FUniqueNetIdRepl & UniqueId, FString & ErrorMessage)
{
    APlayerController* PlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
    
    if (CurGameProgress != EGameProgress::READY)
    {
        ErrorMessage = FString(TEXT("Game is running!"));
        return nullptr;
    }

    AliveClients.Add(Cast<ABRPlayerController>(PlayerController));
    return PlayerController;
}

void ABRGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    AliveClients.Remove(Cast<ABRPlayerController>(Exiting));
}

void ABRGameMode::Broadcast(const FString & Message)
{
    for (int i = 0; i < AliveClients.Num(); ++i)
        if (AliveClients[i])
            AliveClients[i]->MessageToClient(Message);
    
    for (int j = 0; j < DeadClients.Num(); ++j)
        if (DeadClients[j])
            DeadClients[j]->MessageToClient(Message);
}

void ABRGameMode::Dead(ABRPlayerController* PlayerController)
{
    PlayerController->ClientDeathResult(AliveClients.Num());
    AliveClients.Remove(PlayerController);
    DeadClients.Add(PlayerController);
}

#undef LOCTEXT_NAMESPACE

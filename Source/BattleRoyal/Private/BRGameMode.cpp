#include "BRGameMode.h"
#include "BRPlayerController.h"
#include "BRGameState.h"

#define LOCTEXT_NAMESPACE "BRNamespace"

ABRGameMode::ABRGameMode() : CurGameProgress(EGameProgress::READY), MinNumOfPlayer(2), TimeRemaining(10) { }

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
                    if (!--TimeRemaining)
                    {
                        CurGameProgress = EGameProgress::BATTLE;
                        Broadcast(FString(TEXT("Start the battle!")));
                        
                        ABRGameState* BRGameState = Cast<ABRGameState>(GetWorld()->GetGameState());
                        BRGameState->SetDamaged(true);
                        
                        FTimerHandle BroadcastTimerHandle = { };
                        GetWorld()->GetTimerManager().SetTimer(BroadcastTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
                            Broadcast(FString(TEXT("")));
                        }), 1.0f, false, 1.0f);
                    }
                    else
                        Broadcast(FText::Format(LOCTEXT("TimeRemaining", "{0} sec before the battle starts"), TimeRemaining).ToString());
                }
                else
                {
                    TimeRemaining = 10;
                    Broadcast(FString(TEXT("Waiting for another player!")));
                }
                
                break;
            }
            case EGameProgress::BATTLE:
            {
                ABRGameState* BRGameState = Cast<ABRGameState>(GetWorld()->GetGameState());
                BRGameState->SetSurvivor(AliveClients.Num());
                
                if (BRGameState->GetSurvivor() <= 1)
                    CurGameProgress = EGameProgress::RESULT;
                
                break;
            }
            case EGameProgress::RESULT:
            {
                Broadcast(FString(TEXT("Result")));
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
        AliveClients[i]->MessageToClient(Message);
    
    for (int j = 0; j < DeadClients.Num(); ++j)
        DeadClients[j]->MessageToClient(Message);
}

void ABRGameMode::Dead(ABRPlayerController* PlayerController)
{
    AliveClients.Remove(PlayerController);
    DeadClients.Add(PlayerController);
}

#undef LOCTEXT_NAMESPACE

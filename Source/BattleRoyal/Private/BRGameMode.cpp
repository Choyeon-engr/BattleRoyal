#include "BRGameMode.h"
#include "BRPlayerController.h"

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
                break;
            case EGameProgress::RESULT:
                break;
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

    AliveClients.Add(CastChecked<ABRPlayerController>(PlayerController));
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
        AliveClients[i]->PrintMessageToClient(Message);
}

#undef LOCTEXT_NAMESPACE

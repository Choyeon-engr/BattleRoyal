#include "BRGameMode.h"

void ABRGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, FTimerDelegate::CreateLambda([this]() -> void { }), 1.0f, true);
}

APlayerController* ABRGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const class FString & Portal, const class FString & Options, const FUniqueNetIdRepl & UniqueId, class FString & ErrorMessage)
{
    return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

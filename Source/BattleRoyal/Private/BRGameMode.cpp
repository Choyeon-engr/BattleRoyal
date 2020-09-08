#include "BRGameMode.h"

void ABRGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, FTimerDelegate::CreateLambda([this]() -> void { }), 1.0f, true);
}

void ABRGameMode::PreLogin(const class FString & Options, const class FString & Address, const FUniqueNetIdRepl & UniqueId, class FString & ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
    
    if (CurGameProgress != EGameProgress::READY)
        ErrorMessage = FString(TEXT("Game is Running!"));
}

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
    void PrintMessageToClient(const FString & Message);
};

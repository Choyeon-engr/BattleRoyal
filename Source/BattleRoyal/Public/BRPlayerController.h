#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BRPlayerController.generated.h"

UCLASS()
class BATTLEROYAL_API ABRPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintImplementableEvent)
    void PrintMessageToHUD(const FString & Message);
};

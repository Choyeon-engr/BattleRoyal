#pragma once

#include "BattleRoyal.h"
#include "GameFramework/GameStateBase.h"
#include "BRGameState.generated.h"

UCLASS()
class BATTLEROYAL_API ABRGameState : public AGameStateBase
{
	GENERATED_BODY()
    
public:
    FORCEINLINE void SetSurvivor(int32 Personnel) noexcept { Survivor = Personnel; }
    FORCEINLINE int32 GetSurvivor() const noexcept { return Survivor; }
    
protected:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	
private:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    int32 Survivor;
};

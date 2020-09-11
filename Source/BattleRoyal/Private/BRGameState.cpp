#include "BRGameState.h"
#include "Net/UnrealNetwork.h"

void ABRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABRGameState, Survivor);
}

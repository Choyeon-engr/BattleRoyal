#include "BRGameState.h"
#include "Net/UnrealNetwork.h"

void ABRGameState::UpdateCircle()
{
    if (RemainingTime-- <= 0)
    {
        if (!bVisibleNxtCircle)
            bVisibleNxtCircle = true;
        
        if (MovingTime-- <= 0)
        {
            if (!bVisibleCurCircle)
                bVisibleCurCircle = true;
            
            if (ShrinkingTime-- <= 0)
                bVisibleNxtCircle = false;
            else
            {
                CurCircleLocation += DeltaCircleLocation;
                CurCircleRadius += DeltaCircleRadius;
            }
        }
    }
}

void ABRGameState::BeginPlay()
{
    Super::BeginPlay();
    
    DeltaCircleLocation = (NxtCircleLocation - CurCircleLocation) / ShrinkingTime;
    DeltaCircleRadius = (NxtCircleRadius - CurCircleRadius) / (float)ShrinkingTime;
}

void ABRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABRGameState, bDamaged);
    DOREPLIFETIME(ABRGameState, Survivor);
    DOREPLIFETIME(ABRGameState, CurCircleLocation);
    DOREPLIFETIME(ABRGameState, NxtCircleLocation);
    DOREPLIFETIME(ABRGameState, CurCircleRadius);
    DOREPLIFETIME(ABRGameState, NxtCircleRadius);
    DOREPLIFETIME(ABRGameState, bVisibleCurCircle);
    DOREPLIFETIME(ABRGameState, bVisibleNxtCircle);
}

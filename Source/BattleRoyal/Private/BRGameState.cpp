#include "BRGameState.h"
#include "BRMagneticFieldDataTableRow.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"

ABRGameState::ABRGameState() : bDamaged(false), bVisibleCurCircle(false), bVisibleNxtCircle(false), CurMagneticFieldPhase(1)
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/BattleRoyal/DataTable/DT_BRMagneticField"));
    if (DataTable.Succeeded())
        BRMagneticFieldDataTable = DataTable.Object;
}

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
            {
                bVisibleNxtCircle = false;
                SetMagneticFieldPhase(++CurMagneticFieldPhase);
            }
            else
            {
                CurCircleLocation += DeltaCircleLocation;
                CurCircleRadius += DeltaCircleRadius;
            }
        }
    }
}

void ABRGameState::SetMagneticFieldPhase(int32 Phase)
{
    FBRMagneticFieldDataTableRow* BRMagneticFieldDataTableRow = BRMagneticFieldDataTable->FindRow<FBRMagneticFieldDataTableRow>(FName(*(FString::FormatAsNumber(Phase))), FString(""), true);
    
    if (BRMagneticFieldDataTableRow)
    {
        CurMagneticFieldPhase = BRMagneticFieldDataTableRow->GetPhase();
        CurCircleLocation = BRMagneticFieldDataTableRow->GetCurCircleLoc();
        NxtCircleLocation = BRMagneticFieldDataTableRow->GetNxtCircleLoc();
        CurCircleRadius = BRMagneticFieldDataTableRow->GetCurCircleRadius();
        NxtCircleRadius = BRMagneticFieldDataTableRow->GetNxtCircleRadius();
        RemainingTime = BRMagneticFieldDataTableRow->GetRemainingTime();
        MovingTime = BRMagneticFieldDataTableRow->GetMovingTime();
        ShrinkingTime = BRMagneticFieldDataTableRow->GetShrinkingTime();
        
        DeltaCircleLocation = (NxtCircleLocation - CurCircleLocation) / ShrinkingTime;
        DeltaCircleRadius = (NxtCircleRadius - CurCircleRadius) / (float)ShrinkingTime;
    }
}

void ABRGameState::BeginPlay()
{
    Super::BeginPlay();
    
    SetMagneticFieldPhase(CurMagneticFieldPhase);
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

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
                SetMagneticFieldPhase(++CurMagneticFieldPhase, NxtCircleLoc, NxtCircleRadius);
            }
            else
            {
                CurCircleLoc += DeltaCircleLoc;
                CurCircleRadius += DeltaCircleRadius;
            }
        }
    }
}

void ABRGameState::SetMagneticFieldPhase(int32 Phase, FVector PrvCircleLoc, float PrvCircleRadius)
{
    FBRMagneticFieldDataTableRow* BRMagneticFieldDataTableRow = BRMagneticFieldDataTable->FindRow<FBRMagneticFieldDataTableRow>(FName(*(FString::FormatAsNumber(Phase))), FString(""), true);
    
    if (BRMagneticFieldDataTableRow)
    {
        CurMagneticFieldPhase = BRMagneticFieldDataTableRow->GetPhase();
        CurCircleLoc = BRMagneticFieldDataTableRow->GetCurCircleLoc();
        NxtCircleLoc = BRMagneticFieldDataTableRow->GetNxtCircleLoc();
        CurCircleRadius = BRMagneticFieldDataTableRow->GetCurCircleRadius();
        NxtCircleRadius = BRMagneticFieldDataTableRow->GetNxtCircleRadius();
        RemainingTime = BRMagneticFieldDataTableRow->GetRemainingTime();
        MovingTime = BRMagneticFieldDataTableRow->GetMovingTime();
        ShrinkingTime = BRMagneticFieldDataTableRow->GetShrinkingTime();
        
        if (BRMagneticFieldDataTableRow->IsRandomLoc())
        {
            CurCircleLoc = PrvCircleLoc;
            CurCircleRadius = PrvCircleRadius;
            
            FVector RandomVector = FMath::VRand();
            float RandomDistance = FMath::RandRange(BRMagneticFieldDataTableRow->GetNxtMinRandDist(), BRMagneticFieldDataTableRow->GetNxtMaxRandDist());
            
            NxtCircleLoc += RandomVector * RandomDistance;
        }
        
        DeltaCircleLoc = (NxtCircleLoc - CurCircleLoc) / ShrinkingTime;
        DeltaCircleRadius = (NxtCircleRadius - CurCircleRadius) / (float)ShrinkingTime;
    }
}

void ABRGameState::BeginPlay()
{
    Super::BeginPlay();
    
    SetMagneticFieldPhase(CurMagneticFieldPhase, NxtCircleLoc, NxtCircleRadius);
}

void ABRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABRGameState, bDamaged);
    DOREPLIFETIME(ABRGameState, Survivor);
    DOREPLIFETIME(ABRGameState, CurCircleLoc);
    DOREPLIFETIME(ABRGameState, NxtCircleLoc);
    DOREPLIFETIME(ABRGameState, CurCircleRadius);
    DOREPLIFETIME(ABRGameState, NxtCircleRadius);
    DOREPLIFETIME(ABRGameState, bVisibleCurCircle);
    DOREPLIFETIME(ABRGameState, bVisibleNxtCircle);
}

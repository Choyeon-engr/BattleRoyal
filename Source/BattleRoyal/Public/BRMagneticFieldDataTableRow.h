#pragma once

#include "BattleRoyal.h"
#include "Engine/DataTable.h"
#include "BRMagneticFieldDataTableRow.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBRMagneticFieldDataTableRow : public FTableRowBase
{
    GENERATED_BODY()
    
public:
    FORCEINLINE int32 GetPhase() const noexcept { return Phase; }
    FORCEINLINE FVector GetCurCircleLoc() const noexcept { return CurCircleLocation; }
    FORCEINLINE FVector GetNxtCircleLoc() const noexcept { return NxtCircleLocation; }
    FORCEINLINE float GetCurCircleRadius() const noexcept { return CurCircleRadius; }
    FORCEINLINE float GetNxtCircleRadius() const noexcept { return NxtCircleRadius; }
    FORCEINLINE int32 GetRemainingTime() const noexcept { return RemainingTime; }
    FORCEINLINE int32 GetMovingTime() const noexcept { return MovingTime; }
    FORCEINLINE int32 GetShrinkingTime() const noexcept { return ShrinkingTime; }
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 Phase;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    FVector CurCircleLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    FVector NxtCircleLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    float CurCircleRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    float NxtCircleRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 RemainingTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 MovingTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32  ShrinkingTime;
};

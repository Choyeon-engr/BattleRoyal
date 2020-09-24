#pragma once

#include "BattleRoyal.h"
#include "GameFramework/GameStateBase.h"
#include "BRGameState.generated.h"

UCLASS()
class BATTLEROYAL_API ABRGameState : public AGameStateBase
{
	GENERATED_BODY()
    
public:
    ABRGameState();
    
    FORCEINLINE void SetSurvivor(int32 Personnel) noexcept { Survivor = Personnel; }
    FORCEINLINE void SetDamaged(bool IsDamaged) noexcept { bDamaged = IsDamaged; }
    
    FORCEINLINE int32 GetSurvivor() const noexcept { return Survivor; }
    FORCEINLINE bool IsDamaged() const noexcept { return bDamaged; }
    FORCEINLINE bool IsVisibleCurCircle() const noexcept { return bVisibleCurCircle; }
    FORCEINLINE FVector GetCurCircleLoc() const noexcept { return CurCircleLocation; }
    FORCEINLINE float GetCurCircleRadius() const noexcept { return CurCircleRadius; }
    
    void UpdateCircle();
    void SetMagneticFieldPhase(int32 Phase);
    
protected:
    void BeginPlay() override;
    
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	
private:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bDamaged;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    int32 Survivor;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    FVector CurCircleLocation;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    FVector NxtCircleLocation;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float CurCircleRadius;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float NxtCircleRadius;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bVisibleCurCircle;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bVisibleNxtCircle;
    
    int32 RemainingTime;
    int32 MovingTime;
    int32 ShrinkingTime;
    int32 CurMagneticFieldPhase;
    
    FVector DeltaCircleLocation;
    float DeltaCircleRadius;
    
    UPROPERTY()
    class UDataTable* BRMagneticFieldDataTable;
};

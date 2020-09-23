#pragma once

#include "BattleRoyal.h"
#include "GameFramework/GameStateBase.h"
#include "BRGameState.generated.h"

UCLASS()
class BATTLEROYAL_API ABRGameState : public AGameStateBase
{
	GENERATED_BODY()
    
public:
    ABRGameState() : bDamaged(false), CurCircleRadius(12400.0f), NxtCircleRadius(6200.0f), bVisibleCurCircle(false), bVisibleNxtCircle(false), RemainingTime(60), MovingTime(30), ShrinkingTime(15) { }
    
    FORCEINLINE void SetSurvivor(int32 Personnel) noexcept { Survivor = Personnel; }
    FORCEINLINE int32 GetSurvivor() const noexcept { return Survivor; }
    
    FORCEINLINE void SetDamaged(bool IsDamaged) noexcept { bDamaged = IsDamaged; }
    FORCEINLINE bool IsDamaged() const noexcept { return bDamaged; }
    
    void UpdateCircle();
    
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
    
    FVector DeltaCircleLocation;
    
    float DeltaCircleRadius;
};

#pragma once

#include "BattleRoyal.h"
#include "Animation/AnimInstance.h"
#include "BRAnimInstance.generated.h"

UCLASS()
class BATTLEROYAL_API UBRAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
    
protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
private:
    UFUNCTION()
    void AnimNotify_Fire();
    
    UFUNCTION()
    void AnimNotify_Dead();
    
    UFUNCTION()
    void AnimNotify_ResetDamaged();
    
private:
    UPROPERTY(Transient)
    class ABRCharacter* BRCharacter;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float ForwardValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float RightValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bAim;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bDead;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bDamaged;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float LowerBodyRotation;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float CurrentLowerBodyRotation;
};

#pragma once

#include "BattleRoyal.h"
#include "Animation/AnimInstance.h"
#include "BRAnimInstance.generated.h"

UCLASS()
class BATTLEROYAL_API UBRAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
    
protected:
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
private:
    UFUNCTION()
    void AnimNotify_Fire();
    
private:
    UPROPERTY()
    class ABRCharacter* BRCharacter;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float ForwardValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float RightValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bAim;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float LowerBodyRotation;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float CurrentLowerBodyRotation;
};

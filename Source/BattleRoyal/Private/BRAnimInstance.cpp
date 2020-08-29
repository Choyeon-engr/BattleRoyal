#include "BRAnimInstance.h"
#include "BRCharacter.h"

void UBRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    
    BRCharacter = Cast<ABRCharacter>(TryGetPawnOwner());
    
    if (BRCharacter)
    {
        ForwardValue = BRCharacter->GetForwardValue();
        RightValue = BRCharacter->GetRightValue();
        ControllerPitch = BRCharacter->GetControllerPitch();
        
        bAim = BRCharacter->IsAim();
        bDead = BRCharacter->IsDead();
        bDamaged = BRCharacter->IsDamaged();
        
        LowerBodyRotation = RightValue * 30.0f;
        if (!ForwardValue && RightValue)    { ForwardValue = 1.0f; }
        LowerBodyRotation = ForwardValue * RightValue;
        CurrentLowerBodyRotation = FMath::FInterpTo(CurrentLowerBodyRotation, LowerBodyRotation, 1.0f, 0.1f);
    }
}

void UBRAnimInstance::AnimNotify_Fire()
{
    BRCharacter->Fire();
}

void UBRAnimInstance::AnimNotify_Dead()
{
    BRCharacter->Dead();
}

void UBRAnimInstance::AnimNotify_ResetDamaged()
{
    BRCharacter->ResetDamaged();
}

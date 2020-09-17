#include "BRAnimInstance.h"
#include "BRCharacter.h"

UBRAnimInstance::UBRAnimInstance() : bPreFalling(false) { }

void UBRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    
    BRCharacter = Cast<ABRCharacter>(TryGetPawnOwner());
    
    if (BRCharacter)
    {
        ForwardValue = BRCharacter->GetForwardValue();
        RightValue = BRCharacter->GetRightValue();
        LookUpValue = BRCharacter->GetLookUpValue();
        
        bAim = BRCharacter->IsAim();
        bDead = BRCharacter->IsDead();
        bDamaged = BRCharacter->IsDamaged();
        
        if (bAim)
            ControllerPitch = LookUpValue;
        else
            ControllerPitch = 0.0f;
        
        LowerBodyRotation = RightValue * 30.0f;
        if (!ForwardValue && RightValue)
            ForwardValue = 1.0f;
        LowerBodyRotation = ForwardValue * RightValue;
        CurrentLowerBodyRotation = FMath::FInterpTo(CurrentLowerBodyRotation, LowerBodyRotation, 1.0f, 0.1f);
        
        if (!BRCharacter->IsDescent() && !BRCharacter->IsGlid())
        {
            bCurFalling = BRCharacter->GetCharacterMovement()->IsFalling();
            if (bCurFalling && !bPreFalling)
            {
                bStart = true;
                bApex = false;
                bLand = false;
            }
            else if (bCurFalling && bPreFalling)
            {
                bStart = false;
                bApex = true;
                bLand = false;
            }
            else if (!bCurFalling && bPreFalling)
            {
                bStart = false;
                bApex = false;
                bLand = true;
            }
            bPreFalling = bCurFalling;
        }
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

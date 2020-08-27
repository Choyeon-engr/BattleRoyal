#include "BRAnimInstance.h"
#include "BRCharacter.h"

void UBRAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    
    BRCharacter = Cast<ABRCharacter>(TryGetPawnOwner());
}

void UBRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    
    ForwardValue = BRCharacter->GetForwardValue();
    RightValue = BRCharacter->GetRightValue();
    bAim = BRCharacter->IsAim();
    LowerBodyRotation = RightValue * 30.0f;
    if (!ForwardValue && RightValue)    { ForwardValue = 1.0f; }
    LowerBodyRotation = ForwardValue * RightValue;
    CurrentLowerBodyRotation = FMath::FInterpTo(CurrentLowerBodyRotation, LowerBodyRotation, 1.0f, 0.1f);
}

void UBRAnimInstance::AnimNotify_Fire()
{
    BRCharacter->Fire();
}

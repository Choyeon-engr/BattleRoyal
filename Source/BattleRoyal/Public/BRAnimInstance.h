// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BRAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEROYAL_API UBRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
    
public:
    UBRAnimInstance();
    
    FORCEINLINE bool GetAim() const noexcept { return bAim; }
    FORCEINLINE void SetAim(bool isAim) noexcept { bAim = isAim; }
	
private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bAim;
};

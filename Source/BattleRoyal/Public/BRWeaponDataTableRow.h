#pragma once

#include "BattleRoyal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "BRWeaponDataTableRow.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBRWeaponDataTableRow : public FTableRowBase
{
    GENERATED_BODY()
    
public:
    FORCEINLINE int32 GetBRWeaponId() const noexcept { return BRWeaponId; }
    FORCEINLINE FName GetBRWeaponName() const noexcept { return BRWeaponName; }
    FORCEINLINE USkeletalMesh* GetSkeletalMesh() const noexcept { return SkeletalMesh; }
    FORCEINLINE UParticleSystem* GetMuzzleParticle() const  noexcept { return MuzzleParticle; }
    FORCEINLINE USoundCue* GetFireSound() const noexcept { return FireSound; }
    FORCEINLINE int32 GetAttackPower() const noexcept { return AttackPower; }
    FORCEINLINE int32 GetAttackRange() const noexcept { return AttackRange; }
    FORCEINLINE int32 GetBulletQuantity() const noexcept { return BulletQuantity; }
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 BRWeaponId;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    FName BRWeaponName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    USkeletalMesh* SkeletalMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class UParticleSystem* MuzzleParticle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class USoundCue* FireSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 AttackPower;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 AttackRange;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 BulletQuantity;
};

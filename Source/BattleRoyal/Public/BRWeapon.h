#pragma once

#include "BattleRoyal.h"
#include "BRWeapon.generated.h"

UCLASS()
class BATTLEROYAL_API ABRWeapon : public AActor
{
    GENERATED_BODY()
    
public:
    ABRWeapon();
    
    UFUNCTION(BlueprintCallable)
    FORCEINLINE int32 GetBRWeaponId() const noexcept { return BRWeaponId; }
    
    FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const noexcept { return SkeletalMesh; }
    FORCEINLINE UParticleSystem* GetMuzzleParticle() const noexcept { return MuzzleParticle; }
    FORCEINLINE class USoundCue* GetFireSound() const noexcept { return FireSound; }
    FORCEINLINE int32 GetAttackPower() const noexcept { return AttackPower; }
    FORCEINLINE int32 GetAttackRange() const noexcept { return AttackRange; }
    FORCEINLINE int32 GetBulletQuantity() const noexcept { return BulletQuantity; }
    
protected:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    void BeginPlay() override;
    
private:
    void Initialize();
    
private:
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    USkeletalMeshComponent* SkeletalMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class UParticleSystem* MuzzleParticle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class USoundCue* FireSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    bool bRandom;
    
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    int32 BRWeaponId;
    
    FName BRWeaponName;
    int32 AttackPower;
    int32 AttackRange;
    int32 BulletQuantity;
    
    UPROPERTY()
    class UDataTable* BRWeaponDataTable;
};

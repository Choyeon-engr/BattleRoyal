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
    FORCEINLINE class USoundCue* GetReloadSound() const noexcept { return ReloadSound; }
    FORCEINLINE float GetAttackPower() const noexcept { return AttackPower; }
    FORCEINLINE float GetAttackRange() const noexcept { return AttackRange; }
    FORCEINLINE float GetBulletQuantity() const noexcept { return BulletQuantity; }
    
protected:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    void BeginPlay() override;
    
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
    
private:
    void LoadWeapon();
    void Initialize();
    
    UFUNCTION()
    void OnRepBRWeaponId();
    
private:
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    USkeletalMeshComponent* SkeletalMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class UParticleSystem* MuzzleParticle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class USoundCue* FireSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class USoundCue* ReloadSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    bool bRandom;
    
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    int32 WeaponId;
    
    UPROPERTY(ReplicatedUsing = OnRepBRWeaponId)
    int32 BRWeaponId;
    
    FName BRWeaponName;
    float AttackPower;
    float AttackRange;
    float BulletQuantity;
    
    UPROPERTY()
    class UDataTable* BRWeaponDataTable;
};

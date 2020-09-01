#pragma once

#include "BattleRoyal.h"
#include "BRItem.generated.h"

UCLASS()
class BATTLEROYAL_API ABRItem : public AActor
{
    GENERATED_BODY()
    
public:
    ABRItem();
    
    FORCEINLINE int32 GetBRWeaponId() const noexcept { return BRWeaponId; }
    FORCEINLINE USkeletalMesh* GetSkeletalMesh() const noexcept { return SkeletalMesh; }
    
protected:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    void BeginPlay() override;
    
private:
    UFUNCTION()
    void Initialize();
    
private:
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    USphereComponent* Sphere;
    
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    USkeletalMeshComponent* SkeletalMeshComponent;
    
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    USkeletalMesh* SkeletalMesh;
    
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    int32 BRWeaponId;
    
    FName BRWeaponName;
    int32 AttackPower;
    int32 AttackSpeed;
    int32 AttackRange;
    int32 BulletQuantity;
    
    UPROPERTY()
    class UDataTable* BRWeaponDataTable;
};

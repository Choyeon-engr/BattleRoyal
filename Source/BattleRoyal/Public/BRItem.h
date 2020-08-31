#pragma once

#include "BattleRoyal.h"
#include "Engine/DataTable.h"
#include "BRItem.generated.h"

UCLASS()
class BATTLEROYAL_API ABRItem : public AActor
{
    GENERATED_BODY()
    
public:
    ABRItem();
    
protected:
    void BeginPlay() override;
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 BRWeaponId;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    FName BRWeaponName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    USkeletalMesh* SkeletalMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 AttackPower;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 AttackSpeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 AttackRange;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 BulletQuantity;
    
    UDataTable* BRWeaponDataTable;
};

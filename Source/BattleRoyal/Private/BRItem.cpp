#include "BRItem.h"
#include "BRWeaponDataTableRow.h"

ABRItem::ABRItem()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Blueprints/DataTable/DT_BRWeapon"));
    if (DataTable.Succeeded())
        BRWeaponDataTable = DataTable.Object;
}

void ABRItem::BeginPlay()
{
    Super::BeginPlay();
    
    if (BRWeaponId)
    {
        FBRWeaponDataTableRow* BRWeaponDataTableRow = BRWeaponDataTable->FindRow<FBRWeaponDataTableRow>(FName(*(FString::FormatAsNumber(BRWeaponId))), FString(""), true);
        
        BRWeaponId = BRWeaponDataTableRow->GetBRWeaponId();
        BRWeaponName = BRWeaponDataTableRow->GetBRWeaponName();
        SkeletalMesh = BRWeaponDataTableRow->GetSkeletalMesh();
        AttackPower = BRWeaponDataTableRow->GetAttackPower();
        AttackSpeed = BRWeaponDataTableRow->GetAttackSpeed();
        AttackRange = BRWeaponDataTableRow->GetAttackRange();
        BulletQuantity = BRWeaponDataTableRow->GetBulletQuantity();
    }
}

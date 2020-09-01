#include "BRItem.h"
#include "BRWeaponDataTableRow.h"
#include "Engine/DataTable.h"

ABRItem::ABRItem()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Blueprints/DataTable/DT_BRWeapon"));
    if (DataTable.Succeeded())
        BRWeaponDataTable = DataTable.Object;
}

#if WITH_EDITOR
void ABRItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    static const FName & BRWeaponIdName = GET_MEMBER_NAME_CHECKED(ABRItem, BRWeaponId);
    if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == BRWeaponIdName)
        Initialize();
}
#endif

void ABRItem::BeginPlay()
{
    Super::BeginPlay();
    
    if (BRWeaponId)
        Initialize();
}

void ABRItem::Initialize()
{
    FBRWeaponDataTableRow* BRWeaponDataTableRow = BRWeaponDataTable->FindRow<FBRWeaponDataTableRow>(FName(*(FString::FormatAsNumber(BRWeaponId))), FString(""), true);
    
    BRWeaponName = BRWeaponDataTableRow->GetBRWeaponName();
    SkeletalMesh = BRWeaponDataTableRow->GetSkeletalMesh();
    AttackPower = BRWeaponDataTableRow->GetAttackPower();
    AttackSpeed = BRWeaponDataTableRow->GetAttackSpeed();
    AttackRange = BRWeaponDataTableRow->GetAttackRange();
    BulletQuantity = BRWeaponDataTableRow->GetBulletQuantity();
}

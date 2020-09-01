#include "BRItem.h"
#include "BRWeaponDataTableRow.h"
#include "Engine/DataTable.h"

ABRItem::ABRItem()
{
    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    
    Sphere->SetCollisionProfileName(TEXT("BRItem"));
    
    SkeletalMeshComponent->SetupAttachment(Sphere);
    
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
    
    Initialize();
}

void ABRItem::Initialize()
{
    if (BRWeaponId)
    {
        FBRWeaponDataTableRow* BRWeaponDataTableRow = BRWeaponDataTable->FindRow<FBRWeaponDataTableRow>(FName(*(FString::FormatAsNumber(BRWeaponId))), FString(""), true);
        
        BRWeaponName = BRWeaponDataTableRow->GetBRWeaponName();
        SkeletalMeshComponent->SetSkeletalMesh(BRWeaponDataTableRow->GetSkeletalMesh());
        SkeletalMesh = BRWeaponDataTableRow->GetSkeletalMesh();
        AttackPower = BRWeaponDataTableRow->GetAttackPower();
        AttackSpeed = BRWeaponDataTableRow->GetAttackSpeed();
        AttackRange = BRWeaponDataTableRow->GetAttackRange();
        BulletQuantity = BRWeaponDataTableRow->GetBulletQuantity();
    }
    else
    {
        BRWeaponName = FName(TEXT(""));
        SkeletalMeshComponent->SetSkeletalMesh(nullptr);
        SkeletalMesh = nullptr;
        AttackPower = 0;
        AttackSpeed = 0;
        AttackRange = 0;
        BulletQuantity = 0;
    }
}

#include "BRItem.h"
#include "BRWeaponDataTableRow.h"
#include "Engine/DataTable.h"

ABRItem::ABRItem() : bRandom(false)
{
    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    
    Sphere->SetCollisionProfileName(TEXT("BRItem"));
    
    SkeletalMeshComponent->SetupAttachment(Sphere);
    
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/BattleRoyal/DataTable/DT_BRItem"));
    if (DataTable.Succeeded())
        BRWeaponDataTable = DataTable.Object;
}

#if WITH_EDITOR
void ABRItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    static const FName & bRandomName = GET_MEMBER_NAME_CHECKED(ABRItem, bRandom);
    if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == bRandomName)
    {
        static const FName & BRWeaponIdName = GET_MEMBER_NAME_CHECKED(ABRItem, BRWeaponId);
        if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == BRWeaponIdName)
            Initialize();
    }
}
#endif

void ABRItem::BeginPlay()
{
    Super::BeginPlay();
    
    Initialize();
}

void ABRItem::Initialize()
{
    if (bRandom)
        BRWeaponId = FMath::RandRange(1, 10);
    
    if (BRWeaponId)
    {
        FBRWeaponDataTableRow* BRWeaponDataTableRow = BRWeaponDataTable->FindRow<FBRWeaponDataTableRow>(FName(*(FString::FormatAsNumber(BRWeaponId))), FString(""), true);
        
        BRWeaponName = BRWeaponDataTableRow->GetBRWeaponName();
        SkeletalMeshComponent->SetSkeletalMesh(BRWeaponDataTableRow->GetSkeletalMesh());
        SkeletalMesh = BRWeaponDataTableRow->GetSkeletalMesh();
        FireSound = BRWeaponDataTableRow->GetFireSound();
        MuzzleParticle = BRWeaponDataTableRow->GetMuzzleParticle();
        AttackPower = BRWeaponDataTableRow->GetAttackPower();
        AttackRange = BRWeaponDataTableRow->GetAttackRange();
        BulletQuantity = BRWeaponDataTableRow->GetBulletQuantity();
        
        FVector Origin, BoxExtent;
        float SphereRadius;
        UKismetSystemLibrary::GetComponentBounds(SkeletalMeshComponent, Origin, BoxExtent, SphereRadius);
        Sphere->SetSphereRadius(SphereRadius, true);
    }
    else
    {
        BRWeaponName = FName(TEXT(""));
        SkeletalMeshComponent->SetSkeletalMesh(nullptr);
        SkeletalMesh = nullptr;
        AttackPower = 0;
        AttackRange = 0;
        BulletQuantity = 0;
    }
}

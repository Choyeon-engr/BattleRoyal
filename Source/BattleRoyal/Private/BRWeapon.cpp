#include "BRWeapon.h"
#include "BRWeaponDataTableRow.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

ABRWeapon::ABRWeapon() : bRandom(false)
{
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    
    SkeletalMesh->SetCollisionProfileName(TEXT("BRWeapon"));
    SkeletalMesh->SetSimulatePhysics(true);
    
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/BattleRoyal/DataTable/DT_BRWeapon"));
    if (DataTable.Succeeded())
        BRWeaponDataTable = DataTable.Object;
    
    bReplicates = true;
}

#if WITH_EDITOR
void ABRWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    static const FName & bRandomName = GET_MEMBER_NAME_CHECKED(ABRWeapon, bRandom);
    if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == bRandomName)
    {
        static const FName & BRWeaponIdName = GET_MEMBER_NAME_CHECKED(ABRWeapon, BRWeaponId);
        if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == BRWeaponIdName)
            Initialize();
    }
}
#endif

void ABRWeapon::BeginPlay()
{
    Super::BeginPlay();
    
    if (HasAuthority())
        Initialize();
}

void ABRWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABRWeapon, BRWeaponId);
}

void ABRWeapon::LoadWeapon()
{
    if (BRWeaponId)
    {
        FBRWeaponDataTableRow* BRWeaponDataTableRow = BRWeaponDataTable->FindRow<FBRWeaponDataTableRow>(FName(*(FString::FormatAsNumber(BRWeaponId))), FString(""), true);
        
        BRWeaponName = BRWeaponDataTableRow->GetBRWeaponName();
        SkeletalMesh->SetSkeletalMesh(BRWeaponDataTableRow->GetSkeletalMesh());
        MuzzleParticle = BRWeaponDataTableRow->GetMuzzleParticle();
        FireSound = BRWeaponDataTableRow->GetFireSound();
        AttackPower = BRWeaponDataTableRow->GetAttackPower();
        AttackRange = BRWeaponDataTableRow->GetAttackRange();
        BulletQuantity = BRWeaponDataTableRow->GetBulletQuantity();
    }
    else
    {
        BRWeaponName = FName(TEXT(""));
        SkeletalMesh->SetSkeletalMesh(nullptr);
        MuzzleParticle = nullptr;
        FireSound = nullptr;
        AttackPower = 0.0f;
        AttackRange = 0.0f;
        BulletQuantity = 0.0f;
    }
}

void ABRWeapon::Initialize()
{
    if (bRandom)
        BRWeaponId = FMath::RandRange(1, 10);
    
    LoadWeapon();
}

void ABRWeapon::OnRepBRWeaponId()
{
    LoadWeapon();
}

#include "BRCharacter.h"
#include "BRAnimInstance.h"
#include "ParticleDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"

ABRCharacter::ABRCharacter() : bAim(false), bEquipWeapon(false)
{
    PrimaryActorTick.bCanEverTick = true;
    
    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    
    SpringArm->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm);
    
    Camera->SetFieldOfView(70.0f);
    SpringArm->bUsePawnControlRotation = true;
    
    static ConstructorHelpers::FObjectFinder<USoundWave> FIRE_SOUND(TEXT("/Game/SciFiWeapDark/Sound/Rifle/Wavs/RifleA_Fire06"));
    if (FIRE_SOUND.Succeeded())
        FireSound = FIRE_SOUND.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> MUZZLE_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_MuzzleFlash"));
    if (MUZZLE_PARTICLE.Succeeded())
        MuzzleParticle = MUZZLE_PARTICLE.Object;
    
    static ConstructorHelpers::FClassFinder<UUserWidget> CROSSHAIR_CLASS(TEXT("/Game/Blueprints/HUD/BP_HUD_Crosshair.BP_HUD_Crosshair_C"));
    if (CROSSHAIR_CLASS.Succeeded())
        CrosshairClass = CROSSHAIR_CLASS.Class;
}

void ABRCharacter::Fire()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleParticle, GetMesh()->GetSocketLocation(FName(TEXT("Muzzle_01"))), GetActorRotation());
    
    UGameplayStatics::SpawnSoundAtLocation(this, FireSound, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
}

void ABRCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, GetMesh(), FName(TEXT("Muzzle_01")), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, false);
    
    Crosshair = CreateWidget(GetWorld(), CrosshairClass);
}

void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABRCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABRCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABRCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABRCharacter::AddControllerPitchInput);
    
    PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ABRCharacter::Aim);
    PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ABRCharacter::Aim);
    
    PlayerInputComponent->BindAction(TEXT("EquipWeapon"), EInputEvent::IE_Pressed, this, &ABRCharacter::EquipWeapon);
}

void ABRCharacter::MoveForward(const float AxisValue)
{
    ForwardValue = AxisValue;
    AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ABRCharacter::MoveRight(const float AxisValue)
{
    RightValue = AxisValue;
    AddMovementInput(GetActorRightVector(), AxisValue);
}

void ABRCharacter::Aim()
{
    if (bAim)
    {
        bAim = false;
        Camera->SetFieldOfView(70.0f);
        Crosshair->RemoveFromParent();
    }
    else
    {
        bAim = true;
        Camera->SetFieldOfView(35.0f);
        Crosshair->AddToViewport();
    }
}

void ABRCharacter::EquipWeapon()
{
    if (bEquipWeapon)
    {
        bEquipWeapon = false;
        
        Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        
        FName BackpackWeaponSocket = TEXT("backpack_weapon");
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, BackpackWeaponSocket);
    }
    
    else
    {
        bEquipWeapon = true;
        
        Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        
        FName RightHandWeaponSocket = TEXT("weapon_r");
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandWeaponSocket);
    }
}

#include "BRCharacter.h"
#include "BRAnimInstance.h"
#include "ParticleDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABRCharacter::ABRCharacter() : bAim(false), bDead(false), bDamaged(false), bJump(false), bEquipWeapon(false), Health(100.0f), DeadTimer(5.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    
    GetMesh()->SetCollisionProfileName(TEXT("BRCharacter"));
    
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
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> MUZZLE_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_MuzzleFlash"));
    if (MUZZLE_PARTICLE.Succeeded())
        MuzzleParticle = MUZZLE_PARTICLE.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> HIT_WORLD_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitWorld"));
    if (HIT_WORLD_PARTICLE.Succeeded())
        HitWorldParticle = HIT_WORLD_PARTICLE.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> HIT_CHARACTER_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter"));
    if (HIT_CHARACTER_PARTICLE.Succeeded())
        HitCharacterParticle = HIT_CHARACTER_PARTICLE.Object;
    
    static ConstructorHelpers::FClassFinder<UUserWidget> CROSSHAIR_CLASS(TEXT("/Game/Blueprints/HUD/BP_HUD_Crosshair.BP_HUD_Crosshair_C"));
    if (CROSSHAIR_CLASS.Succeeded())
        CrosshairClass = CROSSHAIR_CLASS.Class;
}

void ABRCharacter::Fire()
{
    FHitResult HitResult;
    FCollisionQueryParams Params(FName(TEXT("Bullet")), true, this);
    bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, SpringArm->GetComponentLocation(), SpringArm->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 10000.0f, ECollisionChannel::ECC_GameTraceChannel1, Params);
    auto Target = Cast<ABRCharacter>(HitResult.Actor);
    
    GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleParticle, GetMesh()->GetSocketLocation(FName(TEXT("Muzzle_01"))), GetActorRotation());
    
    UGameplayStatics::SpawnSoundAtLocation(this, FireSound, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
    
    if (bResult)
    {
        if (Target)
        {
            UGameplayStatics::SpawnEmitterAtLocation(this, HitCharacterParticle, HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f, FRotator::ZeroRotator);
            
            UGameplayStatics::ApplyPointDamage(Target, 10.0f, UKismetMathLibrary::GetForwardVector(GetControlRotation()), HitResult, GetController(), this, nullptr);
        }
        else
            UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldParticle, HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f, FRotator::ZeroRotator);
    }
}

void ABRCharacter::Dead()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void { Destroy(); }), DeadTimer, false);
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
    
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABRCharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ABRCharacter::Jump);
    
    PlayerInputComponent->BindAction(TEXT("EquipWeapon"), EInputEvent::IE_Pressed, this, &ABRCharacter::EquipWeapon);
}

float ABRCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    FHitResult HitResult;
    FVector ImpulseDirection;
    DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDirection);
    
    GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    Health -= FinalDamage;
    
    if (HitResult.BoneName == TEXT("head") || Health <= 0.0f)
        bDead = true;
    else
        bDamaged = true;
    
    return FinalDamage;
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

void ABRCharacter::Jump()
{
    if (bJump)
    {
        bJump = false;
        ACharacter::StopJumping();
    }
    else
    {
        bJump = true;
        ACharacter::Jump();
    }
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

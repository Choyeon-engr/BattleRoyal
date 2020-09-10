#include "BRCharacter.h"
#include "BRPlayerController.h"
#include "BRWeapon.h"
#include "BRWeaponDataTableRow.h"
#include "ParticleDefinitions.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ABRCharacter::ABRCharacter() : BRWeapon(nullptr), bAim(false), bDead(false), bDamaged(false), bEquipWeapon(false), bJump(false), Health(100.0f), DeadTimer(5.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    
    GetMesh()->SetCollisionProfileName(TEXT("BRCharacter"));
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    
    SpringArm->SetupAttachment(RootComponent);
    Camera->SetupAttachment(SpringArm);
    
    SpringArm->bUsePawnControlRotation = true;
    Camera->SetFieldOfView(70.0f);
    
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> MUZZLE_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_MuzzleFlash"));
    if (MUZZLE_PARTICLE.Succeeded())
        MuzzleParticle = MUZZLE_PARTICLE.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> HIT_WORLD_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitWorld"));
    if (HIT_WORLD_PARTICLE.Succeeded())
        HitWorldParticle = HIT_WORLD_PARTICLE.Object;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> HIT_CHARACTER_PARTICLE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter"));
    if (HIT_CHARACTER_PARTICLE.Succeeded())
        HitCharacterParticle = HIT_CHARACTER_PARTICLE.Object;
    
    static ConstructorHelpers::FObjectFinder<USoundCue> FIRE_SOUND(TEXT("/Game/SciFiWeapDark/Sound/Rifle/RifleA_Fire_Cue"));
    if (FIRE_SOUND.Succeeded())
        FireSound = FIRE_SOUND.Object;
    
    static ConstructorHelpers::FClassFinder<UUserWidget> CROSSHAIR_C(TEXT("/Game/BattleRoyal/Blueprints/UI/BP_UI_Crosshair.BP_UI_Crosshair_C"));
    if (CROSSHAIR_C.Succeeded())
        CrosshairWidgetClass = CROSSHAIR_C.Class;
}

void ABRCharacter::Fire()
{
    if (!HasAuthority() && BRPlayerController == GetController())
    {
        FHitResult HitResult;
        FCollisionQueryParams Params(FName(TEXT("Bullet")), true, this);
        bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, SpringArm->GetComponentLocation(), SpringArm->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(GetControlRotation()) * (bEquipWeapon ? BRWeapon->GetAttackRange() : 1000), ECollisionChannel::ECC_GameTraceChannel1, Params);
        auto Target = Cast<ABRCharacter>(HitResult.Actor);
        
        BRPlayerController->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
        
        if (bResult)
        {
            if (Target)
            {
                UGameplayStatics::SpawnEmitterAtLocation(this, HitCharacterParticle, HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f, FRotator::ZeroRotator);
                
                ServerApplyPointDamage(Target, (bEquipWeapon ? BRWeapon->GetAttackPower() : 10), UKismetMathLibrary::GetForwardVector(GetControlRotation()), HitResult, GetController(), this, nullptr);
                
                ServerFire(true, HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f);
            }
            else
            {
                UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldParticle, HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f, FRotator::ZeroRotator);
                
                ServerFire(false, HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f);
            }
        }
    }
    
    UGameplayStatics::SpawnEmitterAtLocation(this, (bEquipWeapon ? BRWeapon->GetMuzzleParticle() : MuzzleParticle), GetMesh()->GetSocketLocation(FName(TEXT("Muzzle_01"))), GetActorRotation());
    
    UGameplayStatics::SpawnSoundAtLocation(this, bEquipWeapon ? BRWeapon->GetFireSound() : FireSound, GetActorLocation(), GetActorRotation(), 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
}

void ABRCharacter::Dead()
{
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void { Destroy(); }), DeadTimer, false);
    
    BRPlayerController->Dead();
}

void ABRCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    BRPlayerController = Cast<ABRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    
    if (!HasAuthority() && BRPlayerController == GetController())
        CrosshairWidget = CreateWidget(GetWorld(), CrosshairWidgetClass);
}

void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABRCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABRCharacter::MoveRight);
    
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABRCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABRCharacter::LookUp);
    
    PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ABRCharacter::Aim);
    PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ABRCharacter::Aim);
    
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABRCharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ABRCharacter::Jump);
    
    PlayerInputComponent->BindAction(TEXT("EquipWeapon"), EInputEvent::IE_Pressed, this, &ABRCharacter::EquipWeapon);
    
    PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ABRCharacter::Interaction);
}

float ABRCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    FHitResult HitResult;
    FVector ImpulseDirection;
    DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDirection);
    
    BRPlayerController->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
    
    Health -= FinalDamage;
    
    if (HitResult.BoneName == TEXT("head") || Health <= 0.0f)
        ServerDead();
    else
        ServerDamaged();
    
    return FinalDamage;
}

void ABRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABRCharacter, BRWeapon);
    DOREPLIFETIME(ABRCharacter, ForwardValue);
    DOREPLIFETIME(ABRCharacter, RightValue);
    DOREPLIFETIME(ABRCharacter, LookUpValue);
    DOREPLIFETIME(ABRCharacter, bAim);
    DOREPLIFETIME(ABRCharacter, bDead);
    DOREPLIFETIME(ABRCharacter, bDamaged);
    DOREPLIFETIME(ABRCharacter, bEquipWeapon);
}

ABRWeapon* ABRCharacter::FindWeapon()
{
    if (!HasAuthority() && BRPlayerController == GetController())
    {
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
        
        TArray<AActor*> ActorsToIgnore;
        TArray<class AActor*> OutActors;
        
        bool bResult = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 200.0f, ObjectTypes, ABRWeapon::StaticClass(), ActorsToIgnore, OutActors);
        
        if (bResult)
        {
            for (int32 i = 0; i < OutActors.Num(); ++i)
            {
                ABRWeapon* Weapon = Cast<ABRWeapon>(OutActors[i]);
                USkeletalMeshComponent* SkeletalMesh = Weapon->GetSkeletalMesh();
                
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, false, this);
                
                bool bLineTraceResult = SkeletalMesh->LineTraceComponent(HitResult, SpringArm->GetComponentLocation(), SpringArm->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 200.0f, Params);
                if (bLineTraceResult && SkeletalMesh->IsSimulatingPhysics(FName(TEXT("Root_Bone"))) || SkeletalMesh->IsSimulatingPhysics(FName(TEXT("Root_Bone1"))))
                    return Weapon;
            }
            
            int32 NearestWeaponIndex = -1;
            float NearestWeaponDistance = 200.0f;
            
            for (int32 j = 0; j < OutActors.Num(); ++j)
            {
                ABRWeapon* Weapon = Cast<ABRWeapon>(OutActors[j]);
                float CurWeaponDistance = (OutActors[j]->GetActorLocation() - GetActorLocation()).Size();
                if (NearestWeaponDistance > CurWeaponDistance && Weapon->GetSkeletalMesh()->IsSimulatingPhysics(FName(TEXT("Root_Bone"))) || Weapon->GetSkeletalMesh()->IsSimulatingPhysics(FName(TEXT("Root_Bone1"))))
                {
                    NearestWeaponDistance = CurWeaponDistance;
                    NearestWeaponIndex = j;
                }
            }
            
            if (NearestWeaponIndex != -1)
                return Cast<ABRWeapon>(OutActors[NearestWeaponIndex]);
            else
                return nullptr;
        }
        else
            return nullptr;
    }
    else
        return nullptr;
}

void ABRCharacter::MoveForward(const float AxisValue)
{
    ForwardValue = AxisValue;
    AddMovementInput(GetActorForwardVector(), AxisValue);
    if (ForwardValue != PreForwardValue)
        ServerMoveForward(AxisValue);
    PreForwardValue = ForwardValue;
}

void ABRCharacter::MoveRight(const float AxisValue)
{
    RightValue = AxisValue;
    AddMovementInput(GetActorRightVector(), AxisValue);
    if (RightValue != PreRightValue)
        ServerMoveRight(AxisValue);
    PreRightValue = RightValue;
}

void ABRCharacter::LookUp(const float AxisValue)
{
    AddControllerPitchInput(AxisValue);
    
    LookUpValue = GetControlRotation().Pitch;
    
    if (abs(LookUpValue - PreLookUpValue) > 1.0f)
        ServerLookUp(LookUpValue);
    
    PreLookUpValue = LookUpValue;
}

void ABRCharacter::Aim()
{
    if (bAim)
    {
        bAim = false;
        ServerAim(false);
        Camera->SetFieldOfView(70.0f);
        CrosshairWidget->RemoveFromParent();
    }
    else
    {
        bAim = true;
        ServerAim(true);
        Camera->SetFieldOfView(35.0f);
        CrosshairWidget->AddToViewport();
    }
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

void ABRCharacter::EquipWeapon()
{
    if (bEquipWeapon)
        ServerEquipWeapon(false);
    else
        ServerEquipWeapon(true);
}

void ABRCharacter::Interaction()
{
    if (FindWeapon())
        ServerInteraction(FindWeapon());
}

void ABRCharacter::OnRepBRWeapon()
{
    BRWeapon->GetSkeletalMesh()->SetSimulatePhysics(false);
    
    if (bEquipWeapon)
        BRWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("weapon_r")));
    else
        BRWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("backpack_weapon")));
}

void ABRCharacter::ServerMoveForward_Implementation(const float AxisValue)
{
    ForwardValue = AxisValue;
}

void ABRCharacter::ServerMoveRight_Implementation(const float AxisValue)
{
    RightValue = AxisValue;
}

void ABRCharacter::ServerLookUp_Implementation(const float AxisValue)
{
    LookUpValue = AxisValue;
}

void ABRCharacter::ServerFire_Implementation(bool IsCharacter, FVector SpawnLocation)
{
    MulticastFire(IsCharacter, SpawnLocation);
}

void ABRCharacter::MulticastFire_Implementation(bool IsCharacter, FVector SpawnLocation)
{
    if (!HasAuthority() && BRPlayerController != GetController())
    {
        if (IsCharacter)
            UGameplayStatics::SpawnEmitterAtLocation(this, HitCharacterParticle, SpawnLocation, FRotator::ZeroRotator);
        else
            UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldParticle, SpawnLocation, FRotator::ZeroRotator);
    }
}

void ABRCharacter::ServerApplyPointDamage_Implementation(ABRCharacter* DamagedActor, float BaseDamage, const FVector & HitFromDirection, const FHitResult & HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf <class UDamageType> DamageTypeClass)
{
    UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, HitFromDirection, HitInfo, EventInstigator, DamageCauser, DamageTypeClass);
}

void ABRCharacter::ServerAim_Implementation(bool IsAim)
{
    bAim = IsAim;
}

void ABRCharacter::ServerDead_Implementation()
{
    bDead = true;
    MulticastDead();
}

void ABRCharacter::MulticastDead_Implementation()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABRCharacter::ServerDamaged_Implementation()
{
    bDamaged = true;
}

void ABRCharacter::ServerResetDamaged_Implementation()
{
    bDamaged = false;
}

void ABRCharacter::ServerEquipWeapon_Implementation(bool IsEquipWeapon)
{
    bEquipWeapon = IsEquipWeapon;
}

void ABRCharacter::OnRepEquipWeapon()
{
    if (bEquipWeapon)
        BRWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("weapon_r")));
    else
        BRWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("backpack_weapon")));
}

void ABRCharacter::ServerInteraction_Implementation(ABRWeapon* Weapon)
{
    if (BRWeapon)
        MulticastInteraction();
    
    BRWeapon = Weapon;
}

void ABRCharacter::MulticastInteraction_Implementation()
{
    BRWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    BRWeapon->GetSkeletalMesh()->SetSimulatePhysics(true);
}

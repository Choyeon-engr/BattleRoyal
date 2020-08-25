// Fill out your copyright notice in the Description page of Project Settings.


#include "BRCharacter.h"
#include "BRAnimInstance.h"

// Sets default values
ABRCharacter::ABRCharacter() : bEquipWeapon(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
}

// Called when the game starts or when spawned
void ABRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("EquipWeapon"), EInputEvent::IE_Pressed, this, &ABRCharacter::EquipWeapon);
    
    PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ABRCharacter::Aim);
    PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ABRCharacter::Aim);
}

void ABRCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    BRAnimInstance = Cast<UBRAnimInstance>(GetMesh()->GetAnimInstance());
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

void ABRCharacter::Aim()
{
    if (BRAnimInstance->GetAim())
        BRAnimInstance->SetAim(false);
    else
        BRAnimInstance->SetAim(true);
}

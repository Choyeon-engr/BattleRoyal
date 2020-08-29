#pragma once

#include "BattleRoyal.h"
#include "GameFramework/Character.h"
#include "BRCharacter.generated.h"

UCLASS()
class BATTLEROYAL_API ABRCharacter : public ACharacter
{
    GENERATED_BODY()
    
public:
    ABRCharacter();
    
    void Fire();
    void Dead();
    FORCEINLINE void ResetDamaged() noexcept { bDamaged = false; }
    
    FORCEINLINE float GetForwardValue() const noexcept { return ForwardValue; }
    FORCEINLINE float GetRightValue() const noexcept { return RightValue; }
    FORCEINLINE bool IsAim() const noexcept { return bAim; }
    FORCEINLINE bool IsDead() const noexcept { return bDead; }
    FORCEINLINE bool IsDamaged() const noexcept { return bDamaged; }
    
protected:
    void BeginPlay() override;
    
    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    
private:
    void MoveForward(const float AxisValue);
    void MoveRight(const float AxisValue);
    
    void Aim();
    void EquipWeapon();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    class USkeletalMeshComponent* Weapon;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class UCameraComponent* Camera;
    
    UPROPERTY()
    class USoundWave* FireSound;
    
    UPROPERTY()
    class UParticleSystem* MuzzleParticle;
    
    UPROPERTY()
    class UParticleSystem* HitWorldParticle;
    
    UPROPERTY()
    class UParticleSystem* HitCharacterParticle;
    
    UPROPERTY()
    TSubclassOf<class UUserWidget> CrosshairClass;
    
    UPROPERTY()
    class UUserWidget* Crosshair;
    
    UPROPERTY()
    class UBRAnimInstance* BRAnimInstance;
    
    float ForwardValue;
    float RightValue;
    
    bool bAim;
    bool bDead;
    bool bDamaged;
    bool bEquipWeapon;
    
    float Health;
    float DeadTimer;
    
    FTimerHandle DeadTimerHandle = { };
};

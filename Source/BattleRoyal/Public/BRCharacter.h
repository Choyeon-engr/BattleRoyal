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
    FORCEINLINE float GetControllerPitch() const noexcept { return GetControlRotation().Pitch; }
    
    FORCEINLINE bool IsAim() const noexcept { return bAim; }
    FORCEINLINE bool IsDead() const noexcept { return bDead; }
    FORCEINLINE bool IsDamaged() const noexcept { return bDamaged; }
    
protected:
    void BeginPlay() override;
    
    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
    
private:
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
    ABRWeapon* FindWeapon();
    
    void MoveForward(const float AxisValue);
    void MoveRight(const float AxisValue);
    
    void Aim();
    void Jump();
    void EquipWeapon();
    void Interaction();
    
    UFUNCTION(Server, Unreliable)
    void ServerFire(bool IsCharacter, FVector SpawnLocation);
    
    UFUNCTION(NetMulticast, Unreliable)
    void MulticastFire(bool IsCharacter, FVector SpawnLocation);
    
    UFUNCTION()
    void OnRepBRWeapon();
    
    UFUNCTION(Server, Unreliable)
    void ServerMoveForward(const float AxisValue);
    
    UFUNCTION(Server, Unreliable)
    void ServerMoveRight(const float AxisValue);
    
    UFUNCTION(Server, Reliable)
    void ServerAim(bool IsAim);
    
    UFUNCTION(Server, Reliable)
    void ServerEquipWeapon(bool IsEquipWeapon);
    
    UFUNCTION()
    void OnRepEquipWeapon();
    
    UFUNCTION(Server, Reliable)
    void ServerInteraction(class ABRWeapon* Weapon);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastInteraction();
    
    void ServerFire_Implementation(bool IsCharacter, FVector SpawnLocation);
    void MulticastFire_Implementation(bool IsCharacter, FVector SpawnLocation);
    
    FORCEINLINE void ServerMoveForward_Implementation(const float AxisValue) { ForwardValue = AxisValue; }
    FORCEINLINE void ServerMoveRight_Implementation(const float AxisValue) { RightValue = AxisValue; }
    
    FORCEINLINE void ServerAim_Implementation(bool IsAim) { bAim = IsAim; }
    
    FORCEINLINE void ServerEquipWeapon_Implementation(bool IsEquipWeapon) { bEquipWeapon = IsEquipWeapon; }
    
    void ServerInteraction_Implementation(class ABRWeapon* Weapon);
    void MulticastInteraction_Implementation();
    
private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class UCameraComponent* Camera;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TSubclassOf<UCameraShake> CameraShake;
    
    UPROPERTY(ReplicatedUsing = OnRepBRWeapon)
    class ABRWeapon* BRWeapon;
    
    UPROPERTY()
    class UParticleSystem* MuzzleParticle;
    
    UPROPERTY()
    class UParticleSystem* HitWorldParticle;
    
    UPROPERTY()
    class UParticleSystem* HitCharacterParticle;
    
    UPROPERTY()
    class USoundCue* FireSound;
    
    UPROPERTY()
    TSubclassOf<class UUserWidget> CrosshairClass;
    
    UPROPERTY()
    class UUserWidget* Crosshair;
    
    UPROPERTY(Replicated)
    float ForwardValue;
    
    UPROPERTY(Replicated)
    float RightValue;
    
    UPROPERTY(Replicated)
    bool bAim;
    
    UPROPERTY(ReplicatedUsing = OnRepEquipWeapon)
    bool bEquipWeapon;
    
    bool bDead;
    bool bDamaged;
    bool bJump;
    
    float PreForwardValue;
    float PreRightValue;
    float Health;
    float DeadTimer;
    
    FTimerHandle DeadTimerHandle = { };
};

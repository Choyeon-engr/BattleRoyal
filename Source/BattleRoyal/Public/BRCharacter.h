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
    void Reload();
    void PlayReloadSound();
    void MagneticDamage(float DamageAmount);
    FORCEINLINE void ResetDamaged() { ServerResetDamaged(); }
    
    FORCEINLINE float GetForwardValue() const noexcept { return ForwardValue; }
    FORCEINLINE float GetRightValue() const noexcept { return RightValue; }
    FORCEINLINE float GetLookUpValue() const  noexcept { return LookUpValue; }
    FORCEINLINE float GetControllerPitch() const noexcept { return GetControlRotation().Pitch; }
    
    UFUNCTION(BlueprintCallable)
    FORCEINLINE float GetCurHealth() const noexcept { return CurHealth; }
    
    FORCEINLINE bool IsAim() const noexcept { return bAim; }
    FORCEINLINE bool IsDead() const noexcept { return bDead; }
    FORCEINLINE bool IsDamaged() const noexcept { return bDamaged; }
    FORCEINLINE bool IsDescent() const noexcept { return bDescent; }
    FORCEINLINE bool IsGlid() const noexcept { return bGlid; }
    
    UFUNCTION(BlueprintImplementableEvent)
    void Descent();
    
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void Glid();
    
    UFUNCTION(Server, Reliable)
    void ServerJog();
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastJog();
    
    UFUNCTION(BlueprintImplementableEvent)
    void ResetForJog();
    
    UFUNCTION(BlueprintCallable)
    bool CheckAltitude();
    
protected:
    void BeginPlay() override;
    
    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
    
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
private:
    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
    ABRWeapon* FindWeapon();
    
    void MoveForward(const float AxisValue);
    void MoveRight(const float AxisValue);
    void LookUp(const float AxisValue);
    
    void Aim();
    void Jump();
    void EquipWeapon();
    void Interaction();
    
    UFUNCTION(Server, Unreliable)
    void ServerMoveForward(const float AxisValue);
    
    UFUNCTION(Server, Unreliable)
    void ServerMoveRight(const float AxisValue);
    
    UFUNCTION(Server, Unreliable)
    void ServerLookUp(const float AxisValue);
    
    UFUNCTION(Server, Unreliable)
    void ServerFire(bool IsCharacter, FVector SpawnLocation);
    
    UFUNCTION(NetMulticast, Unreliable)
    void MulticastFire(bool IsCharacter, FVector SpawnLocation);
    
    UFUNCTION(Server, Reliable)
    void ServerApplyPointDamage(ABRCharacter* DamagedActor, float BaseDamage, const FVector & HitFromDirection, const FHitResult & HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf <class UDamageType> DamageTypeClass);
    
    UFUNCTION(Server, Reliable)
    void ServerSetSkin(USkeletalMesh* SkeletalMesh);
    
    UFUNCTION()
    void OnRepSkin();
    
    UFUNCTION()
    void OnRepBRWeapon();
    
    UFUNCTION(Server, Reliable)
    void ServerAim(bool IsAim);
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerReverseSetCanAim();
    
    UFUNCTION(Server, Reliable)
    void ServerDead();
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastDead();
    
    UFUNCTION(Server, Reliable)
    void ServerDamaged();
    
    UFUNCTION(Server, Reliable)
    void ServerResetDamaged();
    
    UFUNCTION(Server, Reliable)
    void ServerEquipWeapon(bool IsEquipWeapon);
    
    UFUNCTION()
    void OnRepEquipWeapon();
    
    UFUNCTION(Server, Reliable)
    void ServerInteraction(class ABRWeapon* Weapon);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastInteraction();
    
    UFUNCTION()
    void OnRepDescent();
    
    UFUNCTION()
    void OnRepGlid();
    
    UFUNCTION(Server, Reliable)
    void ServerGlid();
    
private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class USpringArmComponent* SpringArm;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class UCameraComponent* Camera;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TSubclassOf<UCameraShake> CameraShake;
    
    UPROPERTY(ReplicatedUsing = OnRepSkin)
    USkeletalMesh* Skin;
    
    UPROPERTY(ReplicatedUsing = OnRepBRWeapon)
    class ABRWeapon* BRWeapon;
    
    UPROPERTY()
    class UParticleSystem* MuzzleParticle;
    
    UPROPERTY()
    class UParticleSystem* HitWorldParticle;
    
    UPROPERTY()
    class UParticleSystem* HitCharacterParticle;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class UParticleSystemComponent* CloudParticle;
    
    UPROPERTY()
    class USoundCue* FireSound;
    
    UPROPERTY()
    class USoundCue* ReloadSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    class UAudioComponent* WindSound;
    
    UPROPERTY()
    TSubclassOf<class UUserWidget> CrosshairWidgetClass;
    
    UPROPERTY()
    class UUserWidget* CrosshairWidget;
    
    UPROPERTY(Replicated)
    float ForwardValue;
    
    UPROPERTY(Replicated)
    float RightValue;
    
    UPROPERTY(Replicated)
    float LookUpValue;
    
    UPROPERTY(Replicated)
    float CurHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    int32 BulletQuantity;
    
    UPROPERTY(Replicated)
    bool bAim;
    
    UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    bool bCanAim;
    
    UPROPERTY(Replicated)
    bool bDead;
    
    UPROPERTY(Replicated)
    bool bDamaged;
    
    UPROPERTY(ReplicatedUsing = OnRepEquipWeapon)
    bool bEquipWeapon;
    
    UPROPERTY(ReplicatedUsing = OnRepDescent, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    bool bDescent;
    
    UPROPERTY(ReplicatedUsing = OnRepGlid, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    bool bGlid;
    
    bool bJump;
    
    float PreForwardValue;
    float PreRightValue;
    float PreLookUpValue;
    
    float DeadTimer;
    FTimerHandle DeadTimerHandle = { };
};

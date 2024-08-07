#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "PaperZDAnimInstance.h"
#include "Components/BoxComponent.h"
#include "PlayerHUD.h"
#include "Engine/TimerHandle.h"
#include "SwineEscapeGameInstance.h"
#include "Sound/SoundBase.h"
#include "Items.h"

#include "King_PlayerCharacter.generated.h"

UCLASS()
class SWINEESCAPESAGA_API AKing_PlayerCharacter : public APaperZDCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCameraComponent* PlayerCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* AttackCollision;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UPlayerHUD> KingHudClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPlayerHUD* KingHudWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USwineEscapeGameInstance* GameInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* ItemSFX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool isStunned = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputMappingContext* IMC;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_MoveRight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_Attack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPaperZDAnimSequence* AttackAnimSeq;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_Jump;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool isAbleToMove = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool CanAttack = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsActive = true;

    FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegt;
    FTimerHandle StunTimer;

    FTimerHandle GameOverTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* HealPotionSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* GemsSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* DoubleJumpPotionSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* KeySFX;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool isAlive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int AttackDamage = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int PlayerHP = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackStunDuaration = 0.2f;

    AKing_PlayerCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void MoveRight(const FInputActionValue& Value);
    void JumpBegin(const FInputActionValue& Value);
    void JumpEnds(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void SetDirection(float MovementValueDirection);
    void OnAttackOverrideAnimationEnds(bool Done);

    UFUNCTION()
    void BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent
        , AActor* OtherActor,
        UPrimitiveComponent* OtherComponent,
        int32 OtherBodyIndex, bool bFromSweep
        , const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void EnableAttackCollision(bool Enabled);

    UFUNCTION(BlueprintCallable)
    void TakeDamage(int DamageAmount, float StunDuration);

    UFUNCTION(BlueprintCallable)
    void UpdatePlayerHP(int NewPlayerHP);

    UFUNCTION(BlueprintCallable)
    void Stun(float DurationInSecs);

    void StunTimeOut();

    UFUNCTION(BlueprintCallable)
    void CollectItem(CollectableType ItemType);
    void UnlockDoubleJump();

    void OnGameOverTimeout();
    void StopPlayer();
};

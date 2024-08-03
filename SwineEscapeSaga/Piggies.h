#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Components/SphereComponent.h"
#include "King_PlayerCharacter.h"
#include "Components//TextRenderComponent.h"
#include "PaperZDAnimInstance.h"
#include "Engine/TimerHandle.h"
#include "Components/BoxComponent.h"



#include "Piggies.generated.h"

/**
 *
 */
UCLASS()
class SWINEESCAPESAGA_API APiggies : public APaperZDCharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextRenderComponent* HpPigTxt;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AKing_PlayerCharacter* FollowPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoolDownInSecs = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* AttackCollisionB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PigsHP = 100;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackStunDuration = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAbleToMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanAttack = true;



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSeq;

	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegt;
	FTimerHandle AttackCooldown;
	FTimerHandle StunTime;

	APiggies();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Detectors function to target the player character
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	// func determin if enm should move to the player
	bool SMTT();


	void updateDirection(float MovementDirection);
	
	
	UFUNCTION(BlueprintCallable)
	void updatePigHP(int NewPigHp);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int DamageAmount, float StunDuaration);


	void Stun(float Duration);
	void OnStunTimeOut();

	void Attack();
	void OnAttackCooldownTimeout();
	void OnAttackOverrideAnimEnd(bool Done);

	
	UFUNCTION()
	void BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent
		,AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, 
		int32 OtherBodyIndex, bool bFromSweep
		,const FHitResult& SweepResult);


	UFUNCTION(BlueprintCallable)

	void EnableAttackCollision(bool Enabled);

};

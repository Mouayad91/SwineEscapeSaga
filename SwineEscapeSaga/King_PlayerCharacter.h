// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"




#include "King_PlayerCharacter.generated.h"


/**
 *
 */
UCLASS()
class SWINEESCAPESAGA_API AKing_PlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Jump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isAbleToMove = true;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isAlive = true;





	AKing_PlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveRight(const FInputActionValue& Value);
	void JumpBegin(const FInputActionValue& Value);
	void JumpEnds(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	void SetDirection(float MovementValueDirection);
};
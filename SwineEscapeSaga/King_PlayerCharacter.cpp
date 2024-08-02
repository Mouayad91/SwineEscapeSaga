// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#include "King_PlayerCharacter.h"

AKing_PlayerCharacter::AKing_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void AKing_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AKing_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
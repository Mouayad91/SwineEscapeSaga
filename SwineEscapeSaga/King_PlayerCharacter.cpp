#include "King_PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h" 
#include "Piggies.h"


AKing_PlayerCharacter::AKing_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);




	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision"));
	AttackCollision->SetupAttachment(RootComponent);

}

void AKing_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}


	OnAttackOverrideEndDelegt.BindUObject(this, &AKing_PlayerCharacter::OnAttackOverrideAnimationEnds);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AKing_PlayerCharacter::BeginOverlapAttackBox);

	EnableAttackCollision(false);



}

void AKing_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKing_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &AKing_PlayerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKing_PlayerCharacter::JumpBegin);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AKing_PlayerCharacter::JumpEnds);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &AKing_PlayerCharacter::JumpEnds);
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AKing_PlayerCharacter::Attack);
	}
}

void AKing_PlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();
	if (isAlive && isAbleToMove)
	{
		FVector Direction = FVector(1.0f, 0.f, 0.f);
		AddMovementInput(Direction, MovementValue);
		SetDirection(MovementValue);
	}
}

void AKing_PlayerCharacter::SetDirection(float MovementValueDirection)
{
	if (!Controller) return; 

	FRotator CurrentRotation = Controller->GetControlRotation();

	if (MovementValueDirection < 0.f)
	{
		if (CurrentRotation.Yaw != 180.f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll));
		}
	}
	else if (MovementValueDirection > 0.f) 
	{
		if (CurrentRotation.Yaw != 0.f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
		}
	}
}


void AKing_PlayerCharacter::JumpBegin(const FInputActionValue& Value)
{
	if (isAlive && isAbleToMove)
	{
		Jump();
	}
}

void AKing_PlayerCharacter::JumpEnds(const FInputActionValue& Value)
{
	StopJumping();
}

void AKing_PlayerCharacter::Attack(const FInputActionValue& Value)
{


	if (isAlive && CanAttack) {

		//isAbleToMove = false;
		CanAttack = false;

		EnableAttackCollision(true);
		
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSeq, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegt);

	}






}
void AKing_PlayerCharacter::OnAttackOverrideAnimationEnds(bool Done)
{

	CanAttack = true;
	//isAbleToMove = true;

	EnableAttackCollision(false);


}

void AKing_PlayerCharacter::BeginOverlapAttackBox(
	UPrimitiveComponent* OverlappedComponent ,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{


	APiggies* Piggies = Cast<APiggies>(OtherActor);

	if (Piggies) {

		// apply damage to piggies
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, Piggies->GetName());

	}



}

void AKing_PlayerCharacter::EnableAttackCollision(bool Enabled)
{


	if (Enabled) {

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn
			, ECollisionResponse::ECR_Overlap);


	}
	else {

		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn
			, ECollisionResponse::ECR_Ignore);
	}



}



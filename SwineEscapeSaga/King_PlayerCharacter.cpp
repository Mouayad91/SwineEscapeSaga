#include "King_PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h" 
#include "Piggies.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	GameInstance =	Cast<USwineEscapeGameInstance>(GetGameInstance());

	if (GameInstance) {

		PlayerHP = GameInstance->PlayerHp;
		if (GameInstance->isDoubleJumpUnlocked) {

			UnlockDoubleJump();
		}

	}

	if (KingHudClass)
	{
		KingHudWidget = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), KingHudClass);
		if (KingHudWidget)
		{
			KingHudWidget->AddToPlayerScreen();
			KingHudWidget->SetPlayerHP(PlayerHP);
			KingHudWidget->SetDiamonds(GameInstance->CollectedGemsCount);
			KingHudWidget->SetLevel(GameInstance->currentLevel);
		}
	}




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
	
		EnhancedInputComponent->BindAction(IA_Escape, ETriggerEvent::Triggered, this, &AKing_PlayerCharacter::ExitGame);

	}
}

void AKing_PlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();
	if (isAlive && isAbleToMove && !isStunned)
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
	if (isAlive && isAbleToMove && !isStunned)
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
	if (isAlive && CanAttack && !isStunned) {
		CanAttack = false;
		// Enable attack collision
		EnableAttackCollision(true);
		GetAnimInstance()->PlayAnimationOverride(AttackAnimSeq, FName("DefaultSlot"), 1.0f, 0.0f, OnAttackOverrideEndDelegt);
	}
}

void AKing_PlayerCharacter::OnAttackOverrideAnimationEnds(bool Done)
{
	if (IsActive && isAlive) {
		CanAttack = true;
		//isAbleToMove = true;

	}

	// Disable attack collision
	EnableAttackCollision(false);
}






void AKing_PlayerCharacter::BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APiggies* Piggies = Cast<APiggies>(OtherActor);

	if (Piggies) {
		UE_LOG(LogTemp, Warning, TEXT("Piggies hit detected"));
		Piggies->TakeDamage(AttackDamage, AttackStunDuaration);
	}
}



void AKing_PlayerCharacter::EnableAttackCollision(bool Enabled)
{
	if (Enabled) {
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	else {
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void AKing_PlayerCharacter::TakeDamage(int DamageAmount, float StunDuration)
{


	if (!isAlive) return;
	if (!IsActive) return;
	
	Stun(StunDuration);

	UpdatePlayerHP(PlayerHP - DamageAmount);

	if (PlayerHP <= 0.0f) {

		// player dead
		UpdatePlayerHP(0);
		isAlive = false;
		isAbleToMove = false;
		CanAttack = false;

		GetAnimInstance()->JumpToNode(FName("Death"), FName("LocoMotion"));
		EnableAttackCollision(false);

		float  Delay = 2.0f;
		GetWorldTimerManager().SetTimer(GameOverTimer, this,
			&AKing_PlayerCharacter::OnGameOverTimeout,
			1.0f, false, Delay);
	}
	else
	{

		//player alive
		GetAnimInstance()->JumpToNode(FName("Damaged"), FName("LocoMotion"));


	}

}

void AKing_PlayerCharacter::UpdatePlayerHP(int NewPlayerHP)
{


	PlayerHP = NewPlayerHP;
	GameInstance->SetPlayerHp(PlayerHP);
	KingHudWidget->SetPlayerHP(PlayerHP);


}

void AKing_PlayerCharacter::Stun(float DurationInSecs)
{

	isStunned = true;

	bool isTimerActive = GetWorldTimerManager().IsTimerActive(StunTimer);
	if (isTimerActive) {
		GetWorldTimerManager().ClearTimer(StunTimer);

	}

	GetWorldTimerManager().SetTimer(StunTimer, this, &AKing_PlayerCharacter::StunTimeOut, 1.0f, false, DurationInSecs);

	GetAnimInstance()->StopAllAnimationOverrides();

}

void AKing_PlayerCharacter::StunTimeOut()
{


	isStunned = false;



}

void AKing_PlayerCharacter::CollectItem(CollectableType ItemType)
{
	switch (ItemType)
	{
	case CollectableType::HealPotion:
	{
		if (HealPotionSFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HealPotionSFX);
		}

		int healValue = 20;
		UpdatePlayerHP(PlayerHP + healValue);

	} break;

	case CollectableType::Gems:
	{
		if (GemsSFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), GemsSFX);
		}

		GameInstance->AddGems(1);
		KingHudWidget->SetDiamonds(GameInstance->CollectedGemsCount);


	} break;

	case CollectableType::DoubleJumpPotion:
	{
		if (DoubleJumpPotionSFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DoubleJumpPotionSFX);
		}

		if (!GameInstance->isDoubleJumpUnlocked) {

			GameInstance->isDoubleJumpUnlocked = true;
			UnlockDoubleJump();
		}


	} break;

	case CollectableType::Key:
	{
		if (KeySFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), KeySFX);
		}




	} break;
	default:
	{
	} break;
	}
}

void AKing_PlayerCharacter::UnlockDoubleJump()
{

	JumpMaxCount = 2;

}

void AKing_PlayerCharacter::OnGameOverTimeout()
{


	GameInstance->Restart();



}

void AKing_PlayerCharacter::StopPlayer()
{

	if (IsActive)
	{
		IsActive = false;
		CanAttack = false;
		isAbleToMove = false;

		GetCharacterMovement()->StopMovementImmediately();
	}

}

void AKing_PlayerCharacter::ExitGame()
{

	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0)
		,EQuitPreference::Quit, false);
	

}

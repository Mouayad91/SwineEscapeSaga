#include "Piggies.h"
#include "GameFramework/CharacterMovementComponent.h"

// Constructor
APiggies::APiggies()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the SphereDetector only once
	SphereDetector = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	SphereDetector->SetupAttachment(RootComponent);

	// Collision settings
	SphereDetector->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereDetector->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereDetector->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HpPigTxt = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Piggies HP"));
	HpPigTxt->SetupAttachment(RootComponent);


	AttackCollisionB = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxAttackCollision"));
	AttackCollisionB->SetupAttachment(RootComponent);
}

// BeginPlay
void APiggies::BeginPlay()
{
	Super::BeginPlay();

	SphereDetector->OnComponentBeginOverlap.AddDynamic(this, &APiggies::OnBeginOverlap);
	SphereDetector->OnComponentEndOverlap.AddDynamic(this, &APiggies::OnEndOverlap);

	updatePigHP(PigsHP);
	OnAttackOverrideEndDelegt.BindUObject(this, &APiggies::OnAttackOverrideAnimEnd);;

	AttackCollisionB->OnComponentBeginOverlap.AddDynamic(this, &APiggies::BeginOverlapAttackBox);

	EnableAttackCollision(false);





}

// Tick
void APiggies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAlive && FollowPlayer && !IsStunned)
	{
		float MovementDirection = (FollowPlayer->GetActorLocation().X - GetActorLocation().X) > 0.0f ? 1.0f : -1.0f;
		updateDirection(MovementDirection);
		if (SMTT())
		{
			if (isAbleToMove)
			{
				FVector WorldDirection = FVector(1.0f, 0.0f, 0.0f);
				AddMovementInput(WorldDirection, MovementDirection);
			}
		}
		else
		{
			// Attack player character
			if (FollowPlayer->isAlive) {

				Attack();
			}

		}
	}
}


bool APiggies::SMTT()
{
	bool Result = false;

	if (FollowPlayer)
	{
		float DistanceToPlayer = abs(FollowPlayer->GetActorLocation().X - GetActorLocation().X);
		Result = DistanceToPlayer > StopDistance;
	}

	return Result;
}

void APiggies::updateDirection(float MovementDirection)
{


	FRotator CurrentRotation = GetActorRotation();

	if (MovementDirection < 0.f)
	{
		if (CurrentRotation.Yaw != 180.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll));
		}
	}
	else if (MovementDirection > 0.f)
	{
		if (CurrentRotation.Yaw != 0.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
		}
	}




}



// OnBeginOverlap
void APiggies::OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKing_PlayerCharacter* PlayerCharacter = Cast<AKing_PlayerCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		FollowPlayer = PlayerCharacter;
	}
}

// OnEndOverlap
void APiggies::OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AKing_PlayerCharacter* PlayerCharacter = Cast<AKing_PlayerCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		FollowPlayer = nullptr;
	}
}


void APiggies::updatePigHP(int NewPigHp)
{

	PigsHP = NewPigHp;

	FString Str = FString::Printf(TEXT("HP: %d"), PigsHP);
	HpPigTxt->SetText(FText::FromString(Str));


}

void APiggies::TakeDamage(int DamageAmount, float StunDuaration)
{
	if (!isAlive) return;
	Stun(StunDuaration);

	updatePigHP(PigsHP - DamageAmount);

	if (PigsHP <= 0.0f) {
		// Dead
		updatePigHP(0);
		HpPigTxt->SetHiddenInGame(true);

		isAlive = false;
		isAbleToMove = false;
		CanAttack = false;

		// Death Anim
		UE_LOG(LogTemp, Warning, TEXT("Playing death animation"));
		GetAnimInstance()->JumpToNode(FName("Death"), FName("LocoMotion"));
		EnableAttackCollision(false);
	
	}
	else {
		// Alive

		// Play damage animation
		UE_LOG(LogTemp, Warning, TEXT("Playing damage animation"));
		GetAnimInstance()->JumpToNode(FName("Damaged"), FName("LocoMotion"));
	}
}

void APiggies::Stun(float Duration)
{

	IsStunned = true;

	bool isTimerActive = GetWorldTimerManager().IsTimerActive(StunTime);
	if (isTimerActive) {
		GetWorldTimerManager().ClearTimer(StunTime);

	}
	
	GetWorldTimerManager().SetTimer(StunTime, this, &APiggies::OnStunTimeOut, 1.0f, false, Duration);

	GetAnimInstance()->StopAllAnimationOverrides();
}

void APiggies::OnStunTimeOut()
{


	IsStunned = false;

}

void APiggies::Attack()
{
	if (isAlive && CanAttack && !IsStunned) {
		CanAttack = false;
		isAbleToMove = false;

		GetAnimInstance()->PlayAnimationOverride(AttackAnimSeq,
			FName("DefaultSlot"),
			1.0f,
			0.0f,
			OnAttackOverrideEndDelegt);

		GetWorldTimerManager().SetTimer(AttackCooldown, this, &APiggies::OnAttackCooldownTimeout, AttackCoolDownInSecs, false);
	}
}

void APiggies::OnAttackCooldownTimeout()
{
	if (isAlive) {
		CanAttack = true;
	}
}

void APiggies::OnAttackOverrideAnimEnd(bool Done)
{
	if (isAlive) {
		isAbleToMove = true;
	}
}

void APiggies::BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	AKing_PlayerCharacter* PlayerCharacter = Cast<AKing_PlayerCharacter>(OtherActor);

	if (PlayerCharacter) {

		PlayerCharacter->TakeDamage(AttackDamage, AttackStunDuration);
	}



}

void APiggies::EnableAttackCollision(bool Enabled)
{


	if (Enabled) {
		AttackCollisionB->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollisionB->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	else {
		AttackCollisionB->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollisionB->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}






}

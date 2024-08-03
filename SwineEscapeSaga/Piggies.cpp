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
}

// BeginPlay
void APiggies::BeginPlay()
{
	Super::BeginPlay();

	SphereDetector->OnComponentBeginOverlap.AddDynamic(this, &APiggies::OnBeginOverlap);
	SphereDetector->OnComponentEndOverlap.AddDynamic(this, &APiggies::OnEndOverlap);
}

// Tick
void APiggies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAlive && FollowPlayer)
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

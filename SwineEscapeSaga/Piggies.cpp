
#include "Piggies.h"

APiggies::APiggies()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereDetector = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	SphereDetector->SetupAttachment(RootComponent);
}

void APiggies::BeginPlay()
{
	Super::BeginPlay();

	SphereDetector->OnComponentBeginOverlap.AddDynamic(this, &APiggies::OnBeginOverlap);
	SphereDetector->OnComponentEndOverlap.AddDynamic(this, &APiggies::OnEndOverlap);
}

void APiggies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APiggies::OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APiggies::OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
}

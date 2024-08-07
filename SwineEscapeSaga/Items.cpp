
#include "Items.h"
#include "King_PlayerCharacter.h"

AItems::AItems()
{
	PrimaryActorTick.bCanEverTick = true;


	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComp);


	ItemsFlipbooks = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ItemsFlipbook"));
	ItemsFlipbooks->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AItems::BeginOverlap);

	
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItems::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AKing_PlayerCharacter* Player = Cast<AKing_PlayerCharacter>(OtherActor);

	if (Player && Player->isAlive) {

		Player->CollectItem(Type);
		Destroy();
	}

}


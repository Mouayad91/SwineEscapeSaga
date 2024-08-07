#include "Traps.h"
#include "King_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATraps::ATraps()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    SetRootComponent(BoxComp);

    TrapFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Trap Flipbook"));
    TrapFlipbook->SetupAttachment(RootComponent);

    DamageAmount = 10;

    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATraps::OnOverlapBegin);
}

void ATraps::BeginPlay()
{
    Super::BeginPlay();
}

void ATraps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATraps::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AKing_PlayerCharacter* Player = Cast<AKing_PlayerCharacter>(OtherActor);

    if (Player && Player->isAlive) {
        Player->TakeDamage(DamageAmount, 0.0f); 

        if (TrapHitSound) {
            UGameplayStatics::PlaySound2D(GetWorld(), TrapHitSound);
        }
    }
}

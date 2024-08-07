#include "ProjectileThrowingTrap.h"
#include "TrapProjectile.h"
#include "King_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AProjectileThrowingTrap::AProjectileThrowingTrap()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    SetRootComponent(BoxComp);

    TrapFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Trap Flipbook"));
    TrapFlipbook->SetupAttachment(RootComponent);

    DamageAmount = 10;
    FireInterval = 2.0f;

    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileThrowingTrap::OnOverlapBegin);
}

void AProjectileThrowingTrap::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectileThrowingTrap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectileThrowingTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AKing_PlayerCharacter* Player = Cast<AKing_PlayerCharacter>(OtherActor);

    if (Player && Player->isAlive) {
        if (TrapActivateSound) {
            UGameplayStatics::PlaySound2D(GetWorld(), TrapActivateSound);
        }
        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AProjectileThrowingTrap::FireProjectile, FireInterval, true);
    }
}

void AProjectileThrowingTrap::FireProjectile()
{
    if (ProjectileClass) {
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        ATrapProjectile* Projectile = GetWorld()->SpawnActor<ATrapProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
        if (Projectile) {
            Projectile->DamageAmount = DamageAmount;
        }
    }
}

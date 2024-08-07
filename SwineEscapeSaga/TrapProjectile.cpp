#include "TrapProjectile.h"
#include "King_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ATrapProjectile::ATrapProjectile()
{
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->SetCollisionProfileName("Projectile");
    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    ExplosionFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ExplosionFlipbook"));
    ExplosionFlipbook->SetupAttachment(RootComponent);
    ExplosionFlipbook->SetVisibility(false); // Initially invisible

    DamageAmount = 10;

    CollisionComp->OnComponentHit.AddDynamic(this, &ATrapProjectile::OnHit);
}

void ATrapProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ATrapProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AKing_PlayerCharacter* Player = Cast<AKing_PlayerCharacter>(OtherActor);

    if (Player && Player->isAlive) {
        Player->TakeDamage(DamageAmount, 0.0f); 

        ExplosionFlipbook->SetVisibility(true);
        ExplosionFlipbook->PlayFromStart();

        if (ExplosionSound) {
            UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);
        }

        CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        ProjectileMovement->StopMovementImmediately();

        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATrapProjectile::OnExplosionFinished);
    }
}

void ATrapProjectile::OnExplosionFinished()
{
    Destroy();
}

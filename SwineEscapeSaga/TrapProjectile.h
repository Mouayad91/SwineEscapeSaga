#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"
#include "TrapProjectile.generated.h"

UCLASS()
class SWINEESCAPESAGA_API ATrapProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATrapProjectile();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* ExplosionFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* ExplosionSound;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnExplosionFinished();
};

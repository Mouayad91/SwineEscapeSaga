#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"
#include "ProjectileThrowingTrap.generated.h"

UCLASS()
class SWINEESCAPESAGA_API AProjectileThrowingTrap : public AActor
{
    GENERATED_BODY()

public:
    AProjectileThrowingTrap();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* BoxComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* TrapFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* TrapActivateSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<class ATrapProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireInterval;

protected:
    virtual void BeginPlay() override;

    FTimerHandle FireTimerHandle;

    void FireProjectile();

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

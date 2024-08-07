#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"
#include "Traps.generated.h"

UCLASS()
class SWINEESCAPESAGA_API ATraps : public AActor
{
    GENERATED_BODY()

public:
    

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* BoxComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* TrapFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* TrapHitSound;
   
    
    ATraps();

protected:
    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};


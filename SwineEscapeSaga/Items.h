#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"

#include "Items.generated.h"

UENUM(BlueprintType)
enum class CollectableType : uint8
{
    Gems,
    HealPotion,
    DoubleJumpPotion,
    Key
};

UCLASS()
class SWINEESCAPESAGA_API AItems : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent* CapsuleComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* ItemsFlipbooks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    CollectableType Type;

        

    AItems();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent
        , AActor* OtherActor,
        UPrimitiveComponent* OtherComponent,
        int32 OtherBodyIndex, bool bFromSweep
        , const FHitResult& SweepResult);
};

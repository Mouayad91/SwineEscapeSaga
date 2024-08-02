
#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Components/SphereComponent.h"
#include "King_PlayerCharacter.h"

#include "Piggies.generated.h"

/**
 *
 */
UCLASS()
class SWINEESCAPESAGA_API APiggies : public APaperZDCharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AKing_PlayerCharacter* FollowPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 65.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAbleToMove = true;

	APiggies();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// detectors funcs to target the player character
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};

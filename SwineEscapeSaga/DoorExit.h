
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/TimerHandle.h"

#include "DoorExit.generated.h"

UCLASS()
class SWINEESCAPESAGA_API ADoorExit : public AActor
{
	GENERATED_BODY()
	
public:	
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* DoorFlipbooks;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DoorOpenSfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeInSecs = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive = true;


	FTimerHandle WaitTimer;

	ADoorExit();





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

	UFUNCTION(BlueprintCallable)
	void OnWaitTimeout();
};

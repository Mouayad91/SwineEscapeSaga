#include "DoorExit.h"
#include "kismet/GameplayStatics.h"
#include "King_PlayerCharacter.h"
#include "SwineEscapeGameInstance.h"

ADoorExit::ADoorExit()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);


	DoorFlipbooks = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Door Flipbooks"));
	DoorFlipbooks->SetupAttachment(RootComponent);


	DoorFlipbooks->SetPlayRate(0.0f);
	DoorFlipbooks->SetLooping(false);


}

void ADoorExit::BeginPlay()
{
	Super::BeginPlay();
	

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorExit::BeginOverlap);

	DoorFlipbooks->SetPlaybackPosition(0.0f, false);
}

void ADoorExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorExit::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	AKing_PlayerCharacter* Player = Cast<AKing_PlayerCharacter>(OtherActor);

	if (Player && Player->isAlive) {

		if (IsActive) {

			Player->StopPlayer();

			IsActive = false;
			DoorFlipbooks->SetPlayRate(1.0f);
			DoorFlipbooks->PlayFromStart();
			UGameplayStatics::PlaySound2D(GetWorld(), DoorOpenSfx);
		
			GetWorldTimerManager().SetTimer(
				WaitTimer,
				this,
				&ADoorExit::OnWaitTimeout,
				0.1f, false, WaitTimeInSecs);
		
		
		
		}
	}



}

void ADoorExit::OnWaitTimeout()
{

	USwineEscapeGameInstance* GameInstance = Cast<USwineEscapeGameInstance>(GetGameInstance());

	if (GameInstance) {

		GameInstance->ChangeGameLevel(LevelIndex);

	}

}


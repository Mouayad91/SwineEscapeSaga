// Fill out your copyright notice in the Description page of Project Settings.


#include "SwineEscapeGameInstance.h"
#include "Kismet/GameplayStatics.h"




void USwineEscapeGameInstance::SetPlayerHp(int newHp)
{


	PlayerHp = newHp;


}

void USwineEscapeGameInstance::AddGems(int amount)
{

	CollectedGemsCount += amount;

}

void USwineEscapeGameInstance::ChangeGameLevel(int LevelIndex)
{
	if (LevelIndex <= 0) return;
	currentLevel = LevelIndex;

	FString LevelName = FString::Printf(TEXT("Level_%d"), LevelIndex);
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName));


}

void USwineEscapeGameInstance::Restart()
{

	PlayerHp = 100;
	CollectedGemsCount = 0;
	isDoubleJumpUnlocked = false;

	currentLevel = 1;
	ChangeGameLevel(currentLevel);



}

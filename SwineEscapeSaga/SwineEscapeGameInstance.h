// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SwineEscapeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SWINEESCAPESAGA_API USwineEscapeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerHp = 100;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CollectedGemsCount = 0;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isDoubleJumpUnlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int currentLevel = 1;

	UFUNCTION(BlueprintCallable)
	void SetPlayerHp(int newHp);
	
	UFUNCTION(BlueprintCallable)
	void AddGems(int amount);
	
	UFUNCTION(BlueprintCallable)
	void ChangeGameLevel(int LevelIndex);
	
	UFUNCTION(BlueprintCallable)
	void Restart();





};

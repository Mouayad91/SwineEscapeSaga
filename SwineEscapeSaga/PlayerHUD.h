#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


#include "PlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class SWINEESCAPESAGA_API UPlayerHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* PlayerHP;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* Gems;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* Levels;

    void SetPlayerHP(int newHP);
    void SetDiamonds(int Amount);
    void SetLevel(int index);
};

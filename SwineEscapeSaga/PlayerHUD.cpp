#include "PlayerHUD.h"

void UPlayerHUD::SetPlayerHP(int newHP)
{
    if (IsValid(PlayerHP))
    {
        FString Str = FString::Printf(TEXT("HP: %d"), newHP);
        PlayerHP->SetText(FText::FromString(Str));
    }
}

void UPlayerHUD::SetDiamonds(int Amount)
{
    if (IsValid(Diamonds))
    {
        FString Str = FString::Printf(TEXT("Diamonds: %d"), Amount);
        Diamonds->SetText(FText::FromString(Str));
    }
}

void UPlayerHUD::SetLevel(int index)
{
    if (IsValid(Levels))
    {
        FString Str = FString::Printf(TEXT("Level: %d"), index);
        Levels->SetText(FText::FromString(Str));
    }
}

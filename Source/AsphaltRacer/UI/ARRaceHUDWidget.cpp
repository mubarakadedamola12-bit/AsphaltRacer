#include "ARRaceHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ARArcadeCar.h"
#include "ARRaceManager.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Styling/CoreStyle.h"

void UARRaceHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    WidgetTree->RootWidget = Root;

    SpeedText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SpeedText"));
    SpeedText->SetText(FText::FromString(TEXT("0 mph")));
    SpeedText->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Bold"), 36));
    SpeedText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(SpeedText))
    {
        Slot->SetAnchors(FAnchors(0.f, 1.f, 0.f, 1.f));
        Slot->SetAlignment(FVector2D(0.f, 1.f));
        Slot->SetPosition(FVector2D(40.f, -40.f));
        Slot->SetAutoSize(true);
    }

    NitroBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("NitroBar"));
    NitroBar->SetPercent(1.f);
    NitroBar->SetFillColorAndOpacity(FLinearColor(0.1f, 0.6f, 1.f));
    if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(NitroBar))
    {
        Slot->SetAnchors(FAnchors(0.f, 1.f, 0.f, 1.f));
        Slot->SetAlignment(FVector2D(0.f, 1.f));
        Slot->SetPosition(FVector2D(40.f, -90.f));
        Slot->SetSize(FVector2D(220.f, 18.f));
    }

    LapText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("LapText"));
    LapText->SetText(FText::FromString(TEXT("Lap 1")));
    LapText->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Bold"), 28));
    LapText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(LapText))
    {
        Slot->SetAnchors(FAnchors(1.f, 0.f, 1.f, 0.f));
        Slot->SetAlignment(FVector2D(1.f, 0.f));
        Slot->SetPosition(FVector2D(-40.f, 40.f));
        Slot->SetAutoSize(true);
    }
}

void UARRaceHUDWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    if (SpeedText)
    {
        const int32 SpeedMph = FMath::RoundToInt(GetSpeed() * 0.0223694f);
        SpeedText->SetText(FText::FromString(FString::Printf(TEXT("%d mph"), SpeedMph)));
    }

    if (NitroBar)
    {
        NitroBar->SetPercent(GetNitro() / 100.f);
    }

    if (LapText)
    {
        LapText->SetText(FText::FromString(FString::Printf(TEXT("Lap %d"), GetLap() + 1)));
    }
}

float UARRaceHUDWidget::GetSpeed() const
{
    if (!PlayerCar)
    {
        PlayerCar = Cast<AARArcadeCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (!PlayerCar) return 0.f;
    }

    return PlayerCar->GetVelocity().Size();
}

float UARRaceHUDWidget::GetNitro() const
{
    if (!PlayerCar)
    {
        PlayerCar = Cast<AARArcadeCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (!PlayerCar) return 0.f;
    }

    return PlayerCar->GetNitroAmount();
}

int32 UARRaceHUDWidget::GetLap() const
{
    TArray<AActor*> Managers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARRaceManager::StaticClass(), Managers);

    if (Managers.Num() == 0) return 0;

    AARRaceManager* RM = Cast<AARRaceManager>(Managers[0]);
    return RM ? RM->GetCurrentLap() : 0;
}

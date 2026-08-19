#include "ARGarageWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ARGarageSystem.h"
#include "ARArcadeCar.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Styling/CoreStyle.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

UARGarageWidget::UARGarageWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<USoundBase> ClickSoundFinder(TEXT("/Game/Audio/S_UIClick.S_UIClick"));
    if (ClickSoundFinder.Succeeded())
    {
        ClickSound = ClickSoundFinder.Object;
    }
}

static UButton* MakeLabeledButton(UWidgetTree* WidgetTree, const FName& Name, const FString& Label, float FontSize = 18.f)
{
    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    Button->SetBackgroundColor(FLinearColor(0.15f, 0.15f, 0.18f, 1.f));

    UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), *(Name.ToString() + TEXT("_Text")));
    Text->SetText(FText::FromString(Label));
    Text->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Bold"), FontSize));
    Text->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Text->SetJustification(ETextJustify::Center);
    Button->SetContent(Text);

    return Button;
}

static void ApplyStatsToPossessedCar(UWorld* World, UARGarageSystem* GarageSystem)
{
    if (!World || !GarageSystem) return;

    if (AARArcadeCar* Car = Cast<AARArcadeCar>(UGameplayStatics::GetPlayerPawn(World, 0)))
    {
        const FCarStats Stats = GarageSystem->GetCarStats(GarageSystem->GetCurrentCar());
        Car->ApplyCarStats(Stats.TopSpeed, Stats.Acceleration, Stats.Handling);
        Car->SelectedCarID = GarageSystem->GetCurrentCar();
    }
}

void UARGarageWidget::EnsureGarageSystem()
{
    if (GarageSystem) return;

    if (APlayerController* PC = GetOwningPlayer())
    {
        GarageSystem = PC->FindComponentByClass<UARGarageSystem>();
    }
}

void UARGarageWidget::NativeConstruct()
{
    Super::NativeConstruct();

    EnsureGarageSystem();

    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    WidgetTree->RootWidget = Root;

    UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("GaragePanel"));
    Panel->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.8f));
    Panel->SetPadding(FMargin(30.f));

    if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Panel))
    {
        Slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
        Slot->SetAlignment(FVector2D(0.5f, 0.5f));
        Slot->SetSize(FVector2D(520.f, 420.f));
        Slot->SetPosition(FVector2D::ZeroVector);
    }

    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("GarageBox"));
    Panel->SetContent(Box);

    UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("GarageTitle"));
    Title->SetText(FText::FromString(TEXT("GARAGE")));
    Title->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Bold"), 30));
    Title->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Title->SetJustification(ETextJustify::Center);
    if (UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(Title))
    {
        Slot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    UHorizontalBox* CarRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("CarRow"));
    if (UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(CarRow))
    {
        Slot->SetPadding(FMargin(0.f, 4.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    UButton* StarterButton = MakeLabeledButton(WidgetTree, TEXT("StarterButton"), TEXT("Starter"));
    StarterButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnSelectStarter);
    if (UHorizontalBoxSlot* Slot = CarRow->AddChildToHorizontalBox(StarterButton))
    {
        Slot->SetPadding(FMargin(4.f));
        Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }

    UButton* SpeedsterButton = MakeLabeledButton(WidgetTree, TEXT("SpeedsterButton"), TEXT("Speedster"));
    SpeedsterButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnSelectSpeedster);
    if (UHorizontalBoxSlot* Slot = CarRow->AddChildToHorizontalBox(SpeedsterButton))
    {
        Slot->SetPadding(FMargin(4.f));
        Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }

    UButton* HyperButton = MakeLabeledButton(WidgetTree, TEXT("HyperButton"), TEXT("HyperCar"));
    HyperButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnSelectHyperCar);
    if (UHorizontalBoxSlot* Slot = CarRow->AddChildToHorizontalBox(HyperButton))
    {
        Slot->SetPadding(FMargin(4.f));
        Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }

    StatsText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StatsText"));
    StatsText->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Regular"), 18));
    StatsText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    StatsText->SetJustification(ETextJustify::Center);
    if (UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(StatsText))
    {
        Slot->SetPadding(FMargin(0.f, 16.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    UHorizontalBox* UpgradeRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("UpgradeRow"));
    if (UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(UpgradeRow))
    {
        Slot->SetPadding(FMargin(0.f, 4.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    UButton* UpgradeSpeedButton = MakeLabeledButton(WidgetTree, TEXT("UpgradeSpeedButton"), TEXT("+ Speed"));
    UpgradeSpeedButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnUpgradeSpeedClicked);
    if (UHorizontalBoxSlot* Slot = UpgradeRow->AddChildToHorizontalBox(UpgradeSpeedButton))
    {
        Slot->SetPadding(FMargin(4.f));
        Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }

    UButton* UpgradeAccelButton = MakeLabeledButton(WidgetTree, TEXT("UpgradeAccelButton"), TEXT("+ Accel"));
    UpgradeAccelButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnUpgradeAccelClicked);
    if (UHorizontalBoxSlot* Slot = UpgradeRow->AddChildToHorizontalBox(UpgradeAccelButton))
    {
        Slot->SetPadding(FMargin(4.f));
        Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }

    UButton* UpgradeHandlingButton = MakeLabeledButton(WidgetTree, TEXT("UpgradeHandlingButton"), TEXT("+ Handling"));
    UpgradeHandlingButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnUpgradeHandlingClicked);
    if (UHorizontalBoxSlot* Slot = UpgradeRow->AddChildToHorizontalBox(UpgradeHandlingButton))
    {
        Slot->SetPadding(FMargin(4.f));
        Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }

    UButton* CloseButton = MakeLabeledButton(WidgetTree, TEXT("CloseButton"), TEXT("Close"));
    CloseButton->OnClicked.AddDynamic(this, &UARGarageWidget::OnCloseClicked);
    if (UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(CloseButton))
    {
        Slot->SetPadding(FMargin(0.f, 16.f, 0.f, 0.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    RefreshStatsText();
}

void UARGarageWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    if (GetVisibility() == ESlateVisibility::Visible)
    {
        RefreshStatsText();
    }
}

void UARGarageWidget::RefreshStatsText()
{
    if (!StatsText) return;

    EnsureGarageSystem();
    if (!GarageSystem) return;

    const FName CarID = GarageSystem->GetCurrentCar();
    const FCarStats Stats = GarageSystem->GetCarStats(CarID);

    StatsText->SetText(FText::FromString(FString::Printf(
        TEXT("%s\nTop Speed: %.0f   Accel: %.1f   Handling: %.2f"),
        *CarID.ToString(), Stats.TopSpeed, Stats.Acceleration, Stats.Handling)));
}

void UARGarageWidget::OnSelectStarter() { UGameplayStatics::PlaySound2D(this, ClickSound); SelectCar(FName("StarterCar")); }
void UARGarageWidget::OnSelectSpeedster() { UGameplayStatics::PlaySound2D(this, ClickSound); SelectCar(FName("Speedster")); }
void UARGarageWidget::OnSelectHyperCar() { UGameplayStatics::PlaySound2D(this, ClickSound); SelectCar(FName("HyperCar")); }
void UARGarageWidget::OnUpgradeSpeedClicked() { UGameplayStatics::PlaySound2D(this, ClickSound); UpgradeSpeed(); }
void UARGarageWidget::OnUpgradeAccelClicked() { UGameplayStatics::PlaySound2D(this, ClickSound); UpgradeAcceleration(); }
void UARGarageWidget::OnUpgradeHandlingClicked() { UGameplayStatics::PlaySound2D(this, ClickSound); UpgradeHandling(); }

void UARGarageWidget::OnCloseClicked()
{
    UGameplayStatics::PlaySound2D(this, ClickSound);
    SetVisibility(ESlateVisibility::Collapsed);

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void UARGarageWidget::SelectCar(FName CarID)
{
    EnsureGarageSystem();
    if (!GarageSystem) return;

    GarageSystem->SelectCar(CarID);
    ApplyStatsToPossessedCar(GetWorld(), GarageSystem);
    RefreshStatsText();
}

void UARGarageWidget::UpgradeSpeed()
{
    EnsureGarageSystem();
    if (!GarageSystem) return;
    GarageSystem->UpgradeCar(GarageSystem->GetCurrentCar(), 100.f, 0.f, 0.f);
    ApplyStatsToPossessedCar(GetWorld(), GarageSystem);
    RefreshStatsText();
}

void UARGarageWidget::UpgradeAcceleration()
{
    EnsureGarageSystem();
    if (!GarageSystem) return;
    GarageSystem->UpgradeCar(GarageSystem->GetCurrentCar(), 0.f, 2.f, 0.f);
    ApplyStatsToPossessedCar(GetWorld(), GarageSystem);
    RefreshStatsText();
}

void UARGarageWidget::UpgradeHandling()
{
    EnsureGarageSystem();
    if (!GarageSystem) return;
    GarageSystem->UpgradeCar(GarageSystem->GetCurrentCar(), 0.f, 0.f, 0.05f);
    ApplyStatsToPossessedCar(GetWorld(), GarageSystem);
    RefreshStatsText();
}

float UARGarageWidget::GetTopSpeed(FName CarID) const
{
    if (!GarageSystem) return 0.f;
    return GarageSystem->GetCarStats(CarID).TopSpeed;
}

float UARGarageWidget::GetAcceleration(FName CarID) const
{
    if (!GarageSystem) return 0.f;
    return GarageSystem->GetCarStats(CarID).Acceleration;
}

float UARGarageWidget::GetHandling(FName CarID) const
{
    if (!GarageSystem) return 0.f;
    return GarageSystem->GetCarStats(CarID).Handling;
}

#include "ARMainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Styling/CoreStyle.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

UARMainMenuWidget::UARMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<USoundBase> ClickSoundFinder(TEXT("/Game/Audio/S_UIClick.S_UIClick"));
    if (ClickSoundFinder.Succeeded())
    {
        ClickSound = ClickSoundFinder.Object;
    }
}

static UButton* MakeMenuButton(UWidgetTree* WidgetTree, UVerticalBox* Container, const FName& Name, const FString& Label)
{
    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    Button->SetBackgroundColor(FLinearColor(0.15f, 0.15f, 0.18f, 1.f));

    UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), *(Name.ToString() + TEXT("_Text")));
    Text->SetText(FText::FromString(Label));
    Text->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Bold"), 22));
    Text->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Text->SetJustification(ETextJustify::Center);
    Button->SetContent(Text);

    if (UVerticalBoxSlot* Slot = Container->AddChildToVerticalBox(Button))
    {
        Slot->SetPadding(FMargin(0.f, 8.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    return Button;
}

void UARMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    WidgetTree->RootWidget = Root;

    UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Panel"));
    Panel->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.75f));
    Panel->SetPadding(FMargin(30.f));

    if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Panel))
    {
        Slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
        Slot->SetAlignment(FVector2D(0.5f, 0.5f));
        Slot->SetSize(FVector2D(340.f, 320.f));
        Slot->SetPosition(FVector2D::ZeroVector);
    }

    UVerticalBox* Box = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MenuBox"));
    Panel->SetContent(Box);

    UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
    Title->SetText(FText::FromString(TEXT("PAUSED")));
    Title->SetFont(FCoreStyle::GetDefaultFontStyle(FName("Bold"), 30));
    Title->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Title->SetJustification(ETextJustify::Center);
    if (UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(Title))
    {
        Slot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
        Slot->SetHorizontalAlignment(HAlign_Fill);
    }

    ResumeButton = MakeMenuButton(WidgetTree, Box, TEXT("ResumeButton"), TEXT("Resume"));
    ResumeButton->OnClicked.AddDynamic(this, &UARMainMenuWidget::OnResumeClicked);

    RestartButton = MakeMenuButton(WidgetTree, Box, TEXT("RestartButton"), TEXT("Restart Race"));
    RestartButton->OnClicked.AddDynamic(this, &UARMainMenuWidget::OnRestartClicked);

    QuitButton = MakeMenuButton(WidgetTree, Box, TEXT("QuitButton"), TEXT("Quit"));
    QuitButton->OnClicked.AddDynamic(this, &UARMainMenuWidget::OnQuitClicked);
}

void UARMainMenuWidget::OnResumeClicked()
{
    UGameplayStatics::PlaySound2D(this, ClickSound);
    ResumeGame();
}

void UARMainMenuWidget::OnRestartClicked()
{
    UGameplayStatics::PlaySound2D(this, ClickSound);
    RestartRace();
}

void UARMainMenuWidget::OnQuitClicked()
{
    UGameplayStatics::PlaySound2D(this, ClickSound);
    QuitGame();
}

void UARMainMenuWidget::ResumeGame()
{
    SetVisibility(ESlateVisibility::Collapsed);

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void UARMainMenuWidget::StartRace()
{
    if (!RaceLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, RaceLevelName);
    }
}

void UARMainMenuWidget::RestartRace()
{
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UARMainMenuWidget::QuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

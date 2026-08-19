#include "ARPlayerController.h"
#include "ARGarageSystem.h"
#include "ARProgressionSystem.h"
#include "ARRaceHUDWidget.h"
#include "ARMainMenuWidget.h"
#include "ARGarageWidget.h"
#include "ARArcadeCar.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"

AARPlayerController::AARPlayerController()
{
    bShowMouseCursor = false;

    GarageSystem = CreateDefaultSubobject<UARGarageSystem>(TEXT("GarageSystem"));
    ProgressionSystem = CreateDefaultSubobject<UARProgressionSystem>(TEXT("ProgressionSystem"));

    HUDWidget = nullptr;
    MenuWidget = nullptr;
    GarageWidget = nullptr;
}

void AARPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsLocalController())
    {
        return;
    }

    HUDWidget = CreateWidget<UARRaceHUDWidget>(this, UARRaceHUDWidget::StaticClass());
    if (HUDWidget)
    {
        HUDWidget->AddToViewport(0);
    }

    MenuWidget = CreateWidget<UARMainMenuWidget>(this, UARMainMenuWidget::StaticClass());
    if (MenuWidget)
    {
        MenuWidget->AddToViewport(10);
        MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    GarageWidget = CreateWidget<UARGarageWidget>(this, UARGarageWidget::StaticClass());
    if (GarageWidget)
    {
        GarageWidget->AddToViewport(10);
        GarageWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AARPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (AARArcadeCar* Car = Cast<AARArcadeCar>(InPawn))
    {
        if (GarageSystem)
        {
            const FCarStats Stats = GarageSystem->GetCarStats(GarageSystem->GetCurrentCar());
            Car->ApplyCarStats(Stats.TopSpeed, Stats.Acceleration, Stats.Handling);
            Car->SelectedCarID = GarageSystem->GetCurrentCar();
        }
    }
}

void AARPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Axis mappings set in Config/DefaultInput.ini: "Throttle", "Steer"
    // Action mappings: "Nitro", "Drift", "ToggleMenu", "ToggleGarage"
    InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AARPlayerController::ToggleMenu);
    InputComponent->BindAction("ToggleGarage", IE_Pressed, this, &AARPlayerController::ToggleGarage);
}

void AARPlayerController::ToggleMenu()
{
    if (!MenuWidget) return;

    const bool bWasVisible = MenuWidget->GetVisibility() == ESlateVisibility::Visible;
    MenuWidget->SetVisibility(bWasVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

    SetShowMouseCursor(!bWasVisible);
    if (bWasVisible)
    {
        SetInputMode(FInputModeGameOnly());
    }
    else
    {
        SetInputMode(FInputModeGameAndUI());
    }
}

void AARPlayerController::ToggleGarage()
{
    if (!GarageWidget) return;

    const bool bWasVisible = GarageWidget->GetVisibility() == ESlateVisibility::Visible;
    GarageWidget->SetVisibility(bWasVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

    SetShowMouseCursor(!bWasVisible);
    if (bWasVisible)
    {
        SetInputMode(FInputModeGameOnly());
    }
    else
    {
        SetInputMode(FInputModeGameAndUI());
    }
}

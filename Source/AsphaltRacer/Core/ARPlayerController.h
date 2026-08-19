#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

class UARGarageSystem;
class UARProgressionSystem;
class UARRaceHUDWidget;
class UARMainMenuWidget;
class UARGarageWidget;

UCLASS()
class ASPHALTRACER_API AARPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AARPlayerController();

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

protected:
    virtual void SetupInputComponent() override;

    void ToggleMenu();
    void ToggleGarage();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Systems")
    UARGarageSystem* GarageSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Systems")
    UARProgressionSystem* ProgressionSystem;

    UPROPERTY()
    UARRaceHUDWidget* HUDWidget;

    UPROPERTY()
    UARMainMenuWidget* MenuWidget;

    UPROPERTY()
    UARGarageWidget* GarageWidget;
};

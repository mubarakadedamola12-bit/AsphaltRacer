#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARRaceHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class ASPHALTRACER_API UARRaceHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetSpeed() const;

    UFUNCTION(BlueprintCallable)
    float GetNitro() const;

    UFUNCTION(BlueprintCallable)
    int32 GetLap() const;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    mutable class AARArcadeCar* PlayerCar;

    UPROPERTY()
    UTextBlock* SpeedText;

    UPROPERTY()
    UProgressBar* NitroBar;

    UPROPERTY()
    UTextBlock* LapText;
};

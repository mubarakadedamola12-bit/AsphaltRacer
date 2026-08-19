#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARGarageWidget.generated.h"

class UTextBlock;
class USoundBase;

UCLASS()
class ASPHALTRACER_API UARGarageWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UARGarageWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void SelectCar(FName CarID);

    UFUNCTION(BlueprintCallable)
    void UpgradeSpeed();

    UFUNCTION(BlueprintCallable)
    void UpgradeAcceleration();

    UFUNCTION(BlueprintCallable)
    void UpgradeHandling();

    UFUNCTION(BlueprintCallable)
    float GetTopSpeed(FName CarID) const;

    UFUNCTION(BlueprintCallable)
    float GetAcceleration(FName CarID) const;

    UFUNCTION(BlueprintCallable)
    float GetHandling(FName CarID) const;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    void RefreshStatsText();
    void EnsureGarageSystem();

    UFUNCTION()
    void OnSelectStarter();

    UFUNCTION()
    void OnSelectSpeedster();

    UFUNCTION()
    void OnSelectHyperCar();

    UFUNCTION()
    void OnUpgradeSpeedClicked();

    UFUNCTION()
    void OnUpgradeAccelClicked();

    UFUNCTION()
    void OnUpgradeHandlingClicked();

    UFUNCTION()
    void OnCloseClicked();

    class UARGarageSystem* GarageSystem;

    UPROPERTY()
    UTextBlock* StatsText;

    UPROPERTY()
    USoundBase* ClickSound;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARMainMenuWidget.generated.h"

class UButton;
class USoundBase;

UCLASS()
class ASPHALTRACER_API UARMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UARMainMenuWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category="MainMenu")
    void StartRace();

    UFUNCTION(BlueprintCallable, Category="MainMenu")
    void RestartRace();

    UFUNCTION(BlueprintCallable, Category="MainMenu")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category="MainMenu")
    void QuitGame();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainMenu")
    FName RaceLevelName;

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnRestartClicked();

    UFUNCTION()
    void OnQuitClicked();

    UPROPERTY()
    UButton* ResumeButton;

    UPROPERTY()
    UButton* RestartButton;

    UPROPERTY()
    UButton* QuitButton;

    UPROPERTY()
    USoundBase* ClickSound;
};

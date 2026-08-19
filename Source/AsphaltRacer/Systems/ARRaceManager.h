#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARRaceManager.generated.h"

UCLASS()
class ASPHALTRACER_API AARRaceManager : public AActor
{
    GENERATED_BODY()

public:
    AARRaceManager();

    UFUNCTION(BlueprintCallable)
    void StartRace();

    UFUNCTION(BlueprintCallable)
    void FinishRace(class AARArcadeCar* PlayerCar);

    UFUNCTION(BlueprintCallable)
    int32 GetCurrentLap() const { return CurrentLap; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Race")
    int32 TotalLaps;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Race")
    float CountdownTime;

    int32 CurrentLap;
};

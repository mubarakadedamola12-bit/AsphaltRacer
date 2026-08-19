#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARGarageSystem.generated.h"

USTRUCT(BlueprintType)
struct FCarStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TopSpeed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Acceleration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Handling = 0.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASPHALTRACER_API UARGarageSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UARGarageSystem();

    UFUNCTION(BlueprintCallable)
    void SelectCar(FName CarID);

    UFUNCTION(BlueprintCallable)
    void UpgradeCar(FName CarID, float SpeedBoost, float AccelBoost, float HandlingBoost);

    UFUNCTION(BlueprintCallable)
    FCarStats GetCarStats(FName CarID) const;

    UFUNCTION(BlueprintCallable)
    FName GetCurrentCar() const { return CurrentCar; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Garage")
    FName CurrentCar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Garage")
    TMap<FName, FCarStats> CarStats;
};

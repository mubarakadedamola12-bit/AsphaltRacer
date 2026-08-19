#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARCarData.generated.h"

USTRUCT(BlueprintType)
struct FCarPerformanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TopSpeed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Acceleration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Handling = 0.f;
};

UCLASS(Blueprintable)
class ASPHALTRACER_API UARCarData : public UObject
{
    GENERATED_BODY()

public:
    UARCarData();

    UFUNCTION(BlueprintCallable)
    FCarPerformanceData GetCarStats(FName CarID) const;

    UFUNCTION(BlueprintCallable)
    void UpgradeCar(FName CarID, float SpeedBoost, float AccelBoost, float HandlingBoost);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CarData")
    TMap<FName, FCarPerformanceData> CarStats;
};

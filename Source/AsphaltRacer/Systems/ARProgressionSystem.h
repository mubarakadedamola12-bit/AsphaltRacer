#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARProgressionSystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASPHALTRACER_API UARProgressionSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UARProgressionSystem();

    UFUNCTION(BlueprintCallable)
    void AddXP(int32 Amount);

    UFUNCTION(BlueprintCallable)
    void AddCredits(int32 Amount);

    UFUNCTION(BlueprintCallable)
    bool CanUnlockCar(FName CarID) const;

    UFUNCTION(BlueprintCallable)
    void UnlockCar(FName CarID);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    int32 PlayerXP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    int32 PlayerCredits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    TMap<FName, bool> UnlockedCars;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    TMap<FName, int32> CarUnlockCost;
};

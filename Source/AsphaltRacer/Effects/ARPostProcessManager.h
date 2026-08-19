#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARPostProcessManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASPHALTRACER_API UARPostProcessManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UARPostProcessManager();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Nitro bloom intensity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess")
    float NitroBloomBoost;

    // Speed-based chromatic aberration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess")
    float SpeedChromaticMultiplier;

    // Base values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess")
    float BaseBloom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess")
    float BaseChromatic;

protected:
    class AARArcadeCar* OwnerCar;
    class APostProcessVolume* PostProcessVolume;
};

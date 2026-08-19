#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARCameraEffects.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ASPHALTRACER_API UARCameraEffects : public UActorComponent
{
    GENERATED_BODY()

public:
    UARCameraEffects();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
    float SpeedFOVMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
    float BaseFOV;

protected:
    class AARArcadeCar* OwnerCar;
    class UCameraComponent* Camera;
};

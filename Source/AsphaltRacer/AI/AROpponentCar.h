#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "AROpponentCar.generated.h"

class UStaticMeshComponent;
class UAudioComponent;

UCLASS()
class ASPHALTRACER_API AAROpponentCar : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:
    AAROpponentCar();

    virtual void Tick(float DeltaTime) override;

    void SetThrottleInput(float Value);
    void SetSteeringInput(float Value);

    FVector GetTrackForwardDirection() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    AActor* CurrentTrackSpline;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* Chassis;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* TireFrontLeft;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* TireFrontRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* TireRearLeft;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* TireRearRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UAudioComponent* EngineAudio;
};

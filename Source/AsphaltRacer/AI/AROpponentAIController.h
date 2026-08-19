#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AROpponentAIController.generated.h"

UCLASS()
class ASPHALTRACER_API AAROpponentAIController : public AAIController
{
    GENERATED_BODY()

public:
    AAROpponentAIController();

    virtual void Tick(float DeltaTime) override;
    virtual void OnPossess(APawn* InPawn) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float TargetSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
    float SteeringAggression;

    class AAROpponentCar* ControlledCar;

    void DriveForward(float DeltaTime);
    void SteerTowardsTrack(float DeltaTime);
};

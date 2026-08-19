#include "AROpponentAIController.h"
#include "AROpponentCar.h"
#include "Kismet/GameplayStatics.h"

AAROpponentAIController::AAROpponentAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    TargetSpeed = 2500.f;          // AI cruising speed
    SteeringAggression = 0.6f;     // How sharply AI turns
}

void AAROpponentAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ControlledCar = Cast<AAROpponentCar>(InPawn);
}

void AAROpponentAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!ControlledCar) return;

    DriveForward(DeltaTime);
    SteerTowardsTrack(DeltaTime);
}

void AAROpponentAIController::DriveForward(float DeltaTime)
{
    float CurrentSpeed = ControlledCar->GetVelocity().Size();
    float Throttle = (CurrentSpeed < TargetSpeed) ? 1.0f : 0.2f;

    ControlledCar->SetThrottleInput(Throttle);
}

void AAROpponentAIController::SteerTowardsTrack(float DeltaTime)
{
    FVector Forward = ControlledCar->GetActorForwardVector();
    FVector TrackDirection = ControlledCar->GetTrackForwardDirection();

    float SteeringValue = FVector::DotProduct(Forward, TrackDirection);
    float AdjustedSteer = SteeringValue * SteeringAggression;

    ControlledCar->SetSteeringInput(AdjustedSteer);
}

#include "ARCameraEffects.h"
#include "ARArcadeCar.h"
#include "Camera/CameraComponent.h"

UARCameraEffects::UARCameraEffects()
{
    PrimaryComponentTick.bCanEverTick = true;
    SpeedFOVMultiplier = 0.03f;
    BaseFOV = 90.f;
}

void UARCameraEffects::BeginPlay()
{
    Super::BeginPlay();

    OwnerCar = Cast<AARArcadeCar>(GetOwner());
    if (OwnerCar)
    {
        TArray<UCameraComponent*> Cameras;
        OwnerCar->GetComponents<UCameraComponent>(Cameras);
        if (Cameras.Num() > 0)
        {
            Camera = Cameras[0];
            Camera->SetFieldOfView(BaseFOV);
        }
    }
}

void UARCameraEffects::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!Camera || !OwnerCar) return;

    float Speed = OwnerCar->GetVelocity().Size();
    float TargetFOV = BaseFOV + Speed * SpeedFOVMultiplier;
    float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, 5.f);
    Camera->SetFieldOfView(NewFOV);
}

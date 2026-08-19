#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ARArcadeCar.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UAudioComponent;

UCLASS()
class ASPHALTRACER_API AARArcadeCar : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:
    AARArcadeCar();

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    void SetThrottleInput(float Value);
    void SetSteeringInput(float Value);

    bool IsNitroActive() const { return bNitroActive; }
    float GetNitroAmount() const { return NitroAmount; }

    UFUNCTION(BlueprintCallable, Category="Garage")
    void ApplyCarStats(float TopSpeed, float Acceleration, float Handling);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Garage")
    FName SelectedCarID;

protected:
    void ThrottleInput(float Value);
    void SteerInput(float Value);
    void StartNitro();
    void StopNitro();
    void StartDrift();
    void StopDrift();

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
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UAudioComponent* EngineAudio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Nitro")
    float NitroAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Nitro")
    float NitroDrainRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Nitro")
    float NitroBoostRPM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drift")
    float DriftSteerMultiplier;

    bool bNitroActive;
    bool bIsDrifting;
};

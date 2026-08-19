#include "AROpponentCar.h"
#include "ARWheelFront.h"
#include "ARWheelRear.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Curves/CurveFloat.h"
#include "UObject/ConstructorHelpers.h"

AAROpponentCar::AAROpponentCar()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("/Game/Vehicles/OffroadCar/SKM_Offroad.SKM_Offroad"));
    if (SkeletalMeshFinder.Succeeded())
    {
        GetMesh()->SetSkeletalMeshAsset(SkeletalMeshFinder.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPFinder(TEXT("/Game/Vehicles/OffroadCar/Offroad_AnimBP.Offroad_AnimBP_C"));
    if (AnimBPFinder.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBPFinder.Class);
    }

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(FName("Vehicle"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ChassisMeshFinder(TEXT("/Game/Vehicles/OffroadCar/SM_Offroad_Body.SM_Offroad_Body"));
    Chassis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis"));
    Chassis->SetupAttachment(GetMesh());
    Chassis->SetCollisionProfileName(FName("NoCollision"));
    if (ChassisMeshFinder.Succeeded())
    {
        Chassis->SetStaticMesh(ChassisMeshFinder.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> TireMeshFinder(TEXT("/Game/Vehicles/OffroadCar/SM_Offroad_Tire.SM_Offroad_Tire"));

    TireFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Front Left"));
    TireFrontLeft->SetupAttachment(GetMesh(), FName("VisWheel_FL"));
    TireFrontLeft->SetCollisionProfileName(FName("NoCollision"));

    TireFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Front Right"));
    TireFrontRight->SetupAttachment(GetMesh(), FName("VisWheel_FR"));
    TireFrontRight->SetCollisionProfileName(FName("NoCollision"));
    TireFrontRight->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

    TireRearLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Rear Left"));
    TireRearLeft->SetupAttachment(GetMesh(), FName("VisWheel_BL"));
    TireRearLeft->SetCollisionProfileName(FName("NoCollision"));

    TireRearRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Rear Right"));
    TireRearRight->SetupAttachment(GetMesh(), FName("VisWheel_BR"));
    TireRearRight->SetCollisionProfileName(FName("NoCollision"));
    TireRearRight->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

    if (TireMeshFinder.Succeeded())
    {
        TireFrontLeft->SetStaticMesh(TireMeshFinder.Object);
        TireFrontRight->SetStaticMesh(TireMeshFinder.Object);
        TireRearLeft->SetStaticMesh(TireMeshFinder.Object);
        TireRearRight->SetStaticMesh(TireMeshFinder.Object);
    }

    UChaosWheeledVehicleMovementComponent* ChaosMove = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

    ChaosMove->ChassisHeight = 160.0f;
    ChaosMove->DragCoefficient = 0.1f;
    ChaosMove->DownforceCoefficient = 0.1f;
    ChaosMove->CenterOfMassOverride = FVector(0.0f, 0.0f, 75.0f);
    ChaosMove->bEnableCenterOfMassOverride = true;

    ChaosMove->bLegacyWheelFrictionPosition = false;
    ChaosMove->WheelSetups.SetNum(4);

    ChaosMove->WheelSetups[0].WheelClass = UARWheelFront::StaticClass();
    ChaosMove->WheelSetups[0].BoneName = FName("PhysWheel_FL");

    ChaosMove->WheelSetups[1].WheelClass = UARWheelFront::StaticClass();
    ChaosMove->WheelSetups[1].BoneName = FName("PhysWheel_FR");

    ChaosMove->WheelSetups[2].WheelClass = UARWheelRear::StaticClass();
    ChaosMove->WheelSetups[2].BoneName = FName("PhysWheel_BL");

    ChaosMove->WheelSetups[3].WheelClass = UARWheelRear::StaticClass();
    ChaosMove->WheelSetups[3].BoneName = FName("PhysWheel_BR");

    ChaosMove->EngineSetup.MaxTorque = 600.0f;
    ChaosMove->EngineSetup.MaxRPM = 5000.0f;
    ChaosMove->EngineSetup.EngineIdleRPM = 1200.0f;

    if (FRichCurve* TorqueCurve = ChaosMove->EngineSetup.TorqueCurve.GetRichCurve())
    {
        TorqueCurve->AddKey(0.f, 0.6f);
        TorqueCurve->AddKey(1200.f, 0.8f);
        TorqueCurve->AddKey(3000.f, 1.0f);
        TorqueCurve->AddKey(4500.f, 0.9f);
        TorqueCurve->AddKey(5000.f, 0.5f);
    }

    ChaosMove->TransmissionSetup.bUseAutomaticGears = true;
    ChaosMove->TransmissionSetup.bUseAutoReverse = true;
    ChaosMove->TransmissionSetup.FinalRatio = 2.81f;

    ChaosMove->TransmissionSetup.ForwardGearRatios.SetNum(5);
    ChaosMove->TransmissionSetup.ForwardGearRatios[0] = 4.25f;
    ChaosMove->TransmissionSetup.ForwardGearRatios[1] = 2.52f;
    ChaosMove->TransmissionSetup.ForwardGearRatios[2] = 1.66f;
    ChaosMove->TransmissionSetup.ForwardGearRatios[3] = 1.22f;
    ChaosMove->TransmissionSetup.ForwardGearRatios[4] = 1.0f;

    ChaosMove->TransmissionSetup.ReverseGearRatios.SetNum(1);
    ChaosMove->TransmissionSetup.ReverseGearRatios[0] = 4.04f;

    ChaosMove->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
    ChaosMove->DifferentialSetup.FrontRearSplit = 0.5f;

    ChaosMove->SteeringSetup.SteeringType = ESteeringType::AngleRatio;
    ChaosMove->SteeringSetup.AngleRatio = 0.7f;

    static ConstructorHelpers::FObjectFinder<USoundBase> EngineSoundFinder(TEXT("/Game/Audio/S_EngineLoop.S_EngineLoop"));
    EngineAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
    EngineAudio->SetupAttachment(GetMesh());
    EngineAudio->bAutoActivate = true;
    if (EngineSoundFinder.Succeeded())
    {
        EngineAudio->SetSound(EngineSoundFinder.Object);
    }
}

void AAROpponentCar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (EngineAudio)
    {
        const float Speed = GetVelocity().Size();
        EngineAudio->SetPitchMultiplier(FMath::Clamp(0.7f + Speed / 2500.f, 0.7f, 2.2f));
    }
}

void AAROpponentCar::SetThrottleInput(float Value)
{
    if (UChaosVehicleMovementComponent* MoveComp = GetVehicleMovementComponent())
    {
        MoveComp->SetThrottleInput(Value);
    }
}

void AAROpponentCar::SetSteeringInput(float Value)
{
    if (UChaosVehicleMovementComponent* MoveComp = GetVehicleMovementComponent())
    {
        MoveComp->SetSteeringInput(Value);
    }
}

FVector AAROpponentCar::GetTrackForwardDirection() const
{
    if (!CurrentTrackSpline) return GetActorForwardVector();

    USplineComponent* Spline = CurrentTrackSpline->FindComponentByClass<USplineComponent>();
    if (!Spline) return GetActorForwardVector();

    float Distance = Spline->FindInputKeyClosestToWorldLocation(GetActorLocation());
    return Spline->GetDirectionAtSplineInputKey(Distance, ESplineCoordinateSpace::World);
}

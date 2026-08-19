#include "ARArcadeCar.h"
#include "ARWheelFront.h"
#include "ARWheelRear.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Curves/CurveFloat.h"
#include "UObject/ConstructorHelpers.h"

AARArcadeCar::AARArcadeCar()
{
    PrimaryActorTick.bCanEverTick = true;

    SelectedCarID = FName("Speedster");

    NitroAmount = 100.f;
    NitroDrainRate = 20.f;
    NitroBoostRPM = 3000.f;
    DriftSteerMultiplier = 1.8f;

    bNitroActive = false;
    bIsDrifting = false;

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

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetMesh());
    CameraBoom->TargetArmLength = 650.0f;
    CameraBoom->SocketOffset.Z = 150.0f;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritRoll = false;
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->CameraRotationLagSpeed = 2.0f;
    CameraBoom->CameraLagMaxDistance = 50.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom);

    static ConstructorHelpers::FObjectFinder<USoundBase> EngineSoundFinder(TEXT("/Game/Audio/S_EngineLoop.S_EngineLoop"));
    EngineAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
    EngineAudio->SetupAttachment(GetMesh());
    EngineAudio->bAutoActivate = true;
    if (EngineSoundFinder.Succeeded())
    {
        EngineAudio->SetSound(EngineSoundFinder.Object);
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
    ChaosMove->EngineSetup.EngineBrakeEffect = 0.05f;
    ChaosMove->EngineSetup.EngineRevUpMOI = 5.0f;
    ChaosMove->EngineSetup.EngineRevDownRate = 600.0f;

    if (FRichCurve* TorqueCurve = ChaosMove->EngineSetup.TorqueCurve.GetRichCurve())
    {
        TorqueCurve->AddKey(0.f, 0.6f);
        TorqueCurve->AddKey(1200.f, 0.8f);
        TorqueCurve->AddKey(3000.f, 1.0f);
        TorqueCurve->AddKey(4500.f, 0.9f);
        TorqueCurve->AddKey(6000.f, 0.5f);
        TorqueCurve->AddKey(12000.f, 0.2f);
    }

    ChaosMove->TransmissionSetup.bUseAutomaticGears = true;
    ChaosMove->TransmissionSetup.bUseAutoReverse = true;
    ChaosMove->TransmissionSetup.FinalRatio = 2.81f;
    ChaosMove->TransmissionSetup.ChangeUpRPM = 6000.0f;
    ChaosMove->TransmissionSetup.ChangeDownRPM = 2000.0f;
    ChaosMove->TransmissionSetup.GearChangeTime = 0.2f;
    ChaosMove->TransmissionSetup.TransmissionEfficiency = 0.9f;

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
}

void AARArcadeCar::BeginPlay()
{
    Super::BeginPlay();
}

void AARArcadeCar::ApplyCarStats(float TopSpeed, float Acceleration, float Handling)
{
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        MoveComp->EngineSetup.MaxRPM = FMath::Clamp(TopSpeed * 2.5f, 2000.f, 12000.f);
        MoveComp->EngineSetup.MaxTorque = FMath::Clamp(Acceleration * 45.f, 200.f, 1500.f);
        MoveComp->SteeringSetup.AngleRatio = FMath::Clamp(Handling, 0.1f, 1.5f);
    }
}

void AARArcadeCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Throttle", this, &AARArcadeCar::ThrottleInput);
    PlayerInputComponent->BindAxis("Steer", this, &AARArcadeCar::SteerInput);
    PlayerInputComponent->BindAction("Nitro", IE_Pressed, this, &AARArcadeCar::StartNitro);
    PlayerInputComponent->BindAction("Nitro", IE_Released, this, &AARArcadeCar::StopNitro);
    PlayerInputComponent->BindAction("Drift", IE_Pressed, this, &AARArcadeCar::StartDrift);
    PlayerInputComponent->BindAction("Drift", IE_Released, this, &AARArcadeCar::StopDrift);
}

void AARArcadeCar::ThrottleInput(float Value)
{
    if (auto* MoveComp = GetVehicleMovementComponent())
        MoveComp->SetThrottleInput(Value);
}

void AARArcadeCar::SteerInput(float Value)
{
    if (auto* MoveComp = GetVehicleMovementComponent())
    {
        float Adjusted = bIsDrifting ? Value * DriftSteerMultiplier : Value;
        MoveComp->SetSteeringInput(Adjusted);
    }
}

void AARArcadeCar::StartNitro()
{
    if (NitroAmount > 0.f && !bNitroActive)
    {
        if (auto* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
        {
            MoveComp->EngineSetup.MaxRPM += NitroBoostRPM;
            bNitroActive = true;
        }
    }
}

void AARArcadeCar::StopNitro()
{
    if (bNitroActive)
    {
        if (auto* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
        {
            MoveComp->EngineSetup.MaxRPM -= NitroBoostRPM;
        }
        bNitroActive = false;
    }
}

void AARArcadeCar::StartDrift()
{
    bIsDrifting = true;
}

void AARArcadeCar::StopDrift()
{
    bIsDrifting = false;
}

void AARArcadeCar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bNitroActive)
    {
        NitroAmount = FMath::Max(0.f, NitroAmount - NitroDrainRate * DeltaTime);
        if (NitroAmount <= 0.f)
            StopNitro();
    }

    if (EngineAudio)
    {
        const float Speed = GetVelocity().Size();
        EngineAudio->SetPitchMultiplier(FMath::Clamp(0.7f + Speed / 2500.f, 0.7f, 2.2f));
    }
}

void AARArcadeCar::SetThrottleInput(float Value)
{
    if (auto* MoveComp = GetVehicleMovementComponent())
        MoveComp->SetThrottleInput(Value);
}

void AARArcadeCar::SetSteeringInput(float Value)
{
    if (auto* MoveComp = GetVehicleMovementComponent())
        MoveComp->SetSteeringInput(Value);
}

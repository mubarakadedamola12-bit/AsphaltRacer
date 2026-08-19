#include "ARGameMode.h"
#include "ARRaceManager.h"
#include "ARPlayerController.h"
#include "ARArcadeCar.h"
#include "AROpponentCar.h"
#include "AROpponentAIController.h"
#include "ARTrackManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/SkyAtmosphereComponent.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

AARGameMode::AARGameMode()
{
    DefaultPawnClass = AARArcadeCar::StaticClass();
    PlayerControllerClass = AARPlayerController::StaticClass();

    OpponentCarClass = AAROpponentCar::StaticClass();
    OpponentControllerClass = AAROpponentAIController::StaticClass();
    NumOpponents = 2;

    TrackManager = nullptr;
    PlayerStartSpot = nullptr;
    GroundPlaneMesh = nullptr;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshFinder(TEXT("/Engine/BasicShapes/Plane.Plane"));
    if (PlaneMeshFinder.Succeeded())
    {
        GroundPlaneMesh = PlaneMeshFinder.Object;
    }
}

void AARGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    SpawnEnvironment();
}

void AARGameMode::SpawnEnvironment()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (GroundPlaneMesh)
    {
        if (AStaticMeshActor* Ground = World->SpawnActor<AStaticMeshActor>(FVector::ZeroVector, FRotator::ZeroRotator))
        {
            Ground->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
            Ground->GetStaticMeshComponent()->SetStaticMesh(GroundPlaneMesh);
            Ground->GetStaticMeshComponent()->SetCollisionProfileName(FName("BlockAll"));
            Ground->SetActorScale3D(FVector(250.f, 250.f, 1.f));
        }
    }

    World->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-45.f, 45.f, 0.f));
    World->SpawnActor<ASkyAtmosphere>(FVector::ZeroVector, FRotator::ZeroRotator);
    World->SpawnActor<ASkyLight>(FVector::ZeroVector, FRotator::ZeroRotator);
    World->SpawnActor<AExponentialHeightFog>(FVector::ZeroVector, FRotator::ZeroRotator);

    TrackManager = World->SpawnActor<AARTrackManager>(FVector::ZeroVector, FRotator::ZeroRotator);
    World->SpawnActor<AARRaceManager>(FVector::ZeroVector, FRotator::ZeroRotator);

    if (TrackManager)
    {
        const FTransform StartTransform = TrackManager->GetStartTransform(0);
        PlayerStartSpot = World->SpawnActor<APlayerStart>(StartTransform.GetLocation(), StartTransform.Rotator());
    }
}

void AARGameMode::SpawnOpponents()
{
    if (!TrackManager) return;

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < NumOpponents; ++i)
    {
        const FTransform SpawnTransform = TrackManager->GetStartTransform(i + 1);

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        if (AAROpponentCar* Car = World->SpawnActor<AAROpponentCar>(OpponentCarClass, SpawnTransform, Params))
        {
            if (AAROpponentAIController* AIController = World->SpawnActor<AAROpponentAIController>(OpponentControllerClass))
            {
                AIController->Possess(Car);
            }
        }
    }
}

AActor* AARGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (PlayerStartSpot)
    {
        return PlayerStartSpot;
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}

void AARGameMode::BeginPlay()
{
    Super::BeginPlay();

    SpawnOpponents();

    TArray<AActor*> Managers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARRaceManager::StaticClass(), Managers);
    if (Managers.Num() > 0)
    {
        if (AARRaceManager* RaceManager = Cast<AARRaceManager>(Managers[0]))
        {
            RaceManager->StartRace();
        }
    }
}

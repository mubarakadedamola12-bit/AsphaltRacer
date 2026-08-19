#include "ARTrackManager.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ARCheckpoint.h"
#include "UObject/ConstructorHelpers.h"

AARTrackManager::AARTrackManager()
{
    PrimaryActorTick.bCanEverTick = false;

    TrackSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrackSpline"));
    RootComponent = TrackSpline;

    NextCheckpointIndex = 0;

    TrackRadiusX = 6000.f;
    TrackRadiusY = 3500.f;
    TrackSegmentLength = 1000.f;
    NumCheckpoints = 8;
    TrackSegmentMesh = nullptr;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SegmentMeshFinder(TEXT("/Game/VehicleTemplate/Meshes/SM_Track_10M.SM_Track_10M"));
    if (SegmentMeshFinder.Succeeded())
    {
        TrackSegmentMesh = SegmentMeshFinder.Object;
    }

    BuildOvalSpline();
}

void AARTrackManager::BuildOvalSpline()
{
    TrackSpline->ClearSplinePoints(false);

    const int32 NumPoints = 8;
    for (int32 i = 0; i < NumPoints; ++i)
    {
        const float Angle = FMath::DegreesToRadians(360.f / NumPoints * i);
        const FVector Point(TrackRadiusX * FMath::Cos(Angle), TrackRadiusY * FMath::Sin(Angle), 0.f);
        TrackSpline->AddSplinePoint(Point, ESplineCoordinateSpace::Local, false);
    }

    TrackSpline->SetClosedLoop(true, false);

    for (int32 i = 0; i < TrackSpline->GetNumberOfSplinePoints(); ++i)
    {
        TrackSpline->SetSplinePointType(i, ESplinePointType::Curve, false);
    }

    TrackSpline->UpdateSpline();
}

void AARTrackManager::BeginPlay()
{
    Super::BeginPlay();

    SpawnTrackSegments();
    SpawnCheckpoints();
}

void AARTrackManager::SpawnTrackSegments()
{
    if (!TrackSegmentMesh) return;

    const float TotalLength = TrackSpline->GetSplineLength();
    const int32 NumSegments = FMath::Max(1, FMath::RoundToInt(TotalLength / TrackSegmentLength));
    const float SegLen = TotalLength / NumSegments;

    for (int32 i = 0; i < NumSegments; ++i)
    {
        const float StartDist = SegLen * i;
        const float EndDist = SegLen * (i + 1);

        const FVector StartLoc = TrackSpline->GetLocationAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local);
        const FVector StartDir = TrackSpline->GetDirectionAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local);
        const FVector EndLoc = TrackSpline->GetLocationAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local);
        const FVector EndDir = TrackSpline->GetDirectionAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local);

        USplineMeshComponent* SegmentMesh = NewObject<USplineMeshComponent>(this);
        SegmentMesh->SetMobility(EComponentMobility::Movable);
        SegmentMesh->SetStaticMesh(TrackSegmentMesh);
        SegmentMesh->SetForwardAxis(ESplineMeshAxis::X);
        SegmentMesh->SetStartAndEnd(StartLoc, StartDir * SegLen, EndLoc, EndDir * SegLen);
        SegmentMesh->SetCollisionProfileName(FName("BlockAll"));
        SegmentMesh->SetupAttachment(TrackSpline);
        SegmentMesh->RegisterComponent();
    }
}

void AARTrackManager::SpawnCheckpoints()
{
    const float TotalLength = TrackSpline->GetSplineLength();
    const int32 Count = FMath::Max(1, NumCheckpoints);

    for (int32 i = 0; i < Count; ++i)
    {
        const float Dist = TotalLength * i / Count;
        FTransform SpawnTransform = TrackSpline->GetTransformAtDistanceAlongSpline(Dist, ESplineCoordinateSpace::World);
        SpawnTransform.SetScale3D(FVector::OneVector);

        if (AARCheckpoint* Checkpoint = GetWorld()->SpawnActorDeferred<AARCheckpoint>(AARCheckpoint::StaticClass(), SpawnTransform))
        {
            Checkpoint->SetCheckpointIndex(i);
            Checkpoint->FinishSpawning(SpawnTransform);
        }
    }
}

USplineComponent* AARTrackManager::GetTrackSpline() const
{
    return TrackSpline;
}

void AARTrackManager::RegisterCheckpoint(AARCheckpoint* Checkpoint)
{
    Checkpoints.Add(Checkpoint);
}

void AARTrackManager::PlayerHitCheckpoint(int32 Index)
{
    if (Index == NextCheckpointIndex)
    {
        NextCheckpointIndex++;

        if (NextCheckpointIndex >= Checkpoints.Num())
        {
            NextCheckpointIndex = 0; // Lap complete
            UE_LOG(LogTemp, Log, TEXT("Lap completed!"));
        }
    }
}

FTransform AARTrackManager::GetStartTransform(int32 GridSlot) const
{
    FTransform BaseTransform = TrackSpline->GetTransformAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);
    BaseTransform.SetScale3D(FVector::OneVector);

    const FVector Right = BaseTransform.GetRotation().GetRightVector();
    const FVector Forward = BaseTransform.GetRotation().GetForwardVector();

    const int32 Row = GridSlot / 2;
    const int32 Col = GridSlot % 2;

    const FVector Offset = Right * (Col == 0 ? -300.f : 300.f) - Forward * (Row * 500.f);
    const FVector Location = BaseTransform.GetLocation() + Offset + FVector(0.f, 0.f, 100.f);

    return FTransform(BaseTransform.GetRotation(), Location, FVector::OneVector);
}

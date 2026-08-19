#include "ARCheckpoint.h"
#include "Components/BoxComponent.h"
#include "ARTrackManager.h"
#include "Kismet/GameplayStatics.h"
#include "ARArcadeCar.h"

AARCheckpoint::AARCheckpoint()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetBoxExtent(FVector(100.f, 1200.f, 400.f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AARCheckpoint::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AARCheckpoint::OnTriggerOverlap);

    // Find TrackManager
    TArray<AActor*> Managers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARTrackManager::StaticClass(), Managers);

    if (Managers.Num() > 0)
    {
        TrackManager = Cast<AARTrackManager>(Managers[0]);
        TrackManager->RegisterCheckpoint(this);
    }
}

void AARCheckpoint::OnTriggerOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (Cast<AARArcadeCar>(OtherActor))
    {
        if (TrackManager)
        {
            TrackManager->PlayerHitCheckpoint(CheckpointIndex);
        }
    }
}

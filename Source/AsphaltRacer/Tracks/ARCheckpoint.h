#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARCheckpoint.generated.h"

class UBoxComponent;
class AARTrackManager;

UCLASS()
class ASPHALTRACER_API AARCheckpoint : public AActor
{
    GENERATED_BODY()

public:
    AARCheckpoint();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void SetCheckpointIndex(int32 Index) { CheckpointIndex = Index; }

    UFUNCTION(BlueprintCallable)
    int32 GetCheckpointIndex() const { return CheckpointIndex; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Checkpoint")
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Checkpoint")
    int32 CheckpointIndex;

    AARTrackManager* TrackManager;

    UFUNCTION()
    void OnTriggerOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};

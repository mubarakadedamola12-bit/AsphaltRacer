#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARTrackManager.generated.h"

class USplineComponent;
class UStaticMesh;
class UMaterialInterface;

UCLASS()
class ASPHALTRACER_API AARTrackManager : public AActor
{
    GENERATED_BODY()

public:
    AARTrackManager();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    USplineComponent* GetTrackSpline() const;

    UFUNCTION(BlueprintCallable)
    void RegisterCheckpoint(class AARCheckpoint* Checkpoint);

    UFUNCTION(BlueprintCallable)
    void PlayerHitCheckpoint(int32 Index);

    UFUNCTION(BlueprintCallable)
    FTransform GetStartTransform(int32 GridSlot) const;

protected:
    void BuildOvalSpline();
    void SpawnTrackSegments();
    void SpawnCheckpoints();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Track")
    USplineComponent* TrackSpline;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Track")
    TArray<class AARCheckpoint*> Checkpoints;

    UPROPERTY(EditAnywhere, Category="Track")
    float TrackRadiusX;

    UPROPERTY(EditAnywhere, Category="Track")
    float TrackRadiusY;

    UPROPERTY(EditAnywhere, Category="Track")
    float TrackSegmentLength;

    UPROPERTY(EditAnywhere, Category="Track")
    int32 NumCheckpoints;

    UPROPERTY(EditAnywhere, Category="Track")
    UStaticMesh* TrackSegmentMesh;

    int32 NextCheckpointIndex;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARGameMode.generated.h"

class AARTrackManager;
class AAROpponentCar;
class AAROpponentAIController;
class UStaticMesh;

UCLASS()
class ASPHALTRACER_API AARGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AARGameMode();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
    virtual void BeginPlay() override;

    void SpawnEnvironment();
    void SpawnOpponents();

    UPROPERTY()
    AARTrackManager* TrackManager;

    UPROPERTY()
    AActor* PlayerStartSpot;

    UPROPERTY()
    UStaticMesh* GroundPlaneMesh;

    UPROPERTY(EditAnywhere, Category="Race")
    int32 NumOpponents;

    UPROPERTY(EditAnywhere, Category="Race")
    TSubclassOf<AAROpponentCar> OpponentCarClass;

    UPROPERTY(EditAnywhere, Category="Race")
    TSubclassOf<AAROpponentAIController> OpponentControllerClass;
};

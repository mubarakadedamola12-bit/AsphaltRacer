#include "ARRaceManager.h"
#include "Kismet/GameplayStatics.h"
#include "ARArcadeCar.h"

AARRaceManager::AARRaceManager()
{
    PrimaryActorTick.bCanEverTick = false;

    TotalLaps = 3;
    CountdownTime = 3.f;
    CurrentLap = 0;
}

void AARRaceManager::BeginPlay()
{
    Super::BeginPlay();
}

void AARRaceManager::StartRace()
{
    UE_LOG(LogTemp, Log, TEXT("Race starting in %.1f seconds"), CountdownTime);
}

void AARRaceManager::FinishRace(AARArcadeCar* PlayerCar)
{
    UE_LOG(LogTemp, Log, TEXT("Race finished!"));
}

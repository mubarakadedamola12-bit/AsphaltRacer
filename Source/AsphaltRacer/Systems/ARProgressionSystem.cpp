#include "ARProgressionSystem.h"

UARProgressionSystem::UARProgressionSystem()
{
    PrimaryComponentTick.bCanEverTick = false;

    PlayerXP = 0;
    PlayerCredits = 0;

    CarUnlockCost.Add("StarterCar", 0);
    CarUnlockCost.Add("Speedster", 500);
    CarUnlockCost.Add("HyperCar", 2000);
}

void UARProgressionSystem::AddXP(int32 Amount)
{
    PlayerXP += Amount;
}

void UARProgressionSystem::AddCredits(int32 Amount)
{
    PlayerCredits += Amount;
}

bool UARProgressionSystem::CanUnlockCar(FName CarID) const
{
    if (!CarUnlockCost.Contains(CarID)) return false;
    return PlayerCredits >= CarUnlockCost[CarID];
}

void UARProgressionSystem::UnlockCar(FName CarID)
{
    if (CanUnlockCar(CarID))
    {
        PlayerCredits -= CarUnlockCost[CarID];
        UnlockedCars.Add(CarID, true);
    }
}

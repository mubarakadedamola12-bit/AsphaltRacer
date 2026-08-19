#include "ARGarageSystem.h"

UARGarageSystem::UARGarageSystem()
{
    PrimaryComponentTick.bCanEverTick = false;

    CarStats.Add("StarterCar", { 1500.f, 10.f, 0.8f });
    CarStats.Add("Speedster", { 2200.f, 14.f, 0.9f });
    CarStats.Add("HyperCar", { 3000.f, 20.f, 1.0f });

    CurrentCar = "StarterCar";
}

void UARGarageSystem::SelectCar(FName CarID)
{
    if (CarStats.Contains(CarID))
    {
        CurrentCar = CarID;
    }
}

void UARGarageSystem::UpgradeCar(FName CarID, float SpeedBoost, float AccelBoost, float HandlingBoost)
{
    if (CarStats.Contains(CarID))
    {
        FCarStats& Stats = CarStats[CarID];
        Stats.TopSpeed += SpeedBoost;
        Stats.Acceleration += AccelBoost;
        Stats.Handling += HandlingBoost;
    }
}

FCarStats UARGarageSystem::GetCarStats(FName CarID) const
{
    if (CarStats.Contains(CarID))
    {
        return CarStats[CarID];
    }

    return FCarStats();
}

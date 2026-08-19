#include "ARCarData.h"

UARCarData::UARCarData()
{
    CarStats.Add("StarterCar", {1500.f, 10.f, 0.8f});
    CarStats.Add("Speedster", {2200.f, 14.f, 0.9f});
    CarStats.Add("HyperCar", {3000.f, 20.f, 1.0f});
}

FCarPerformanceData UARCarData::GetCarStats(FName CarID) const
{
    if (CarStats.Contains(CarID))
        return CarStats[CarID];

    return FCarPerformanceData();
}

void UARCarData::UpgradeCar(FName CarID, float SpeedBoost, float AccelBoost, float HandlingBoost)
{
    if (CarStats.Contains(CarID))
    {
        FCarPerformanceData& Stats = CarStats[CarID];
        Stats.TopSpeed += SpeedBoost;
        Stats.Acceleration += AccelBoost;
        Stats.Handling += HandlingBoost;
    }
}

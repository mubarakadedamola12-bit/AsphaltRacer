#include "ARPostProcessManager.h"
#include "ARArcadeCar.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"

UARPostProcessManager::UARPostProcessManager()
{
    PrimaryComponentTick.bCanEverTick = true;

    NitroBloomBoost = 2.0f;
    SpeedChromaticMultiplier = 0.004f;

    BaseBloom = 1.0f;
    BaseChromatic = 0.0f;
}

void UARPostProcessManager::BeginPlay()
{
    Super::BeginPlay();

    OwnerCar = Cast<AARArcadeCar>(GetOwner());

    // Find global post-process volume
    TArray<AActor*> Volumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), Volumes);

    if (Volumes.Num() > 0)
    {
        PostProcessVolume = Cast<APostProcessVolume>(Volumes[0]);
    }
}

void UARPostProcessManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!OwnerCar || !PostProcessVolume) return;

    float Speed = OwnerCar->GetVelocity().Size();

    // Nitro bloom
    float BloomIntensity = BaseBloom;
    if (OwnerCar->GetVelocity().Size() > 2000 && OwnerCar->IsNitroActive()) // You can add IsNitroActive() later
    {
        BloomIntensity += NitroBloomBoost;
    }

    // Speed-based chromatic aberration
    float ChromaticAmount = BaseChromatic + Speed * SpeedChromaticMultiplier;

    // Apply to post-process settings
    FPostProcessSettings& Settings = PostProcessVolume->Settings;

    Settings.bOverride_BloomIntensity = 1;
    Settings.BloomIntensity = BloomIntensity;

    Settings.bOverride_SceneFringeIntensity = 1;
    Settings.SceneFringeIntensity = ChromaticAmount;
}

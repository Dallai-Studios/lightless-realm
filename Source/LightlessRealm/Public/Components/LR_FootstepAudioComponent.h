#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LR_FootstepAudioComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTLESSREALM_API ULR_FootstepAudioComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Component Config")
	class USoundBase* grassFootstepSounds;

	UPROPERTY(EditAnywhere, Category="Component Config")
	class USoundAttenuation* attenuationSettings;
	
	UPROPERTY(EditAnywhere, Category="Component Config")
	float floorDetectionRaySize = 100;


	
public:
	// ======================================================
	// Ciclo de vida:
	// ======================================================
	ULR_FootstepAudioComponent();
	virtual void BeginPlay() override;


	
	// ======================================================
	// Metodos de som de passos:
	// ======================================================
	UFUNCTION(BlueprintCallable, Category="Character Step Sounds")
	void PlayFootstepSoundAtActorLocation();
};

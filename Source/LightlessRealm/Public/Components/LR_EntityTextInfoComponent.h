#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ELRTextContentAnimationDirection.h"
#include "LR_EntityTextInfoComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTLESSREALM_API ULR_EntityTextInfoComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// ===========================================
	// Configurações:
	// ===========================================
	UPROPERTY(EditAnywhere, Category="Component Config")
	TSubclassOf<class ALR_EntityGameTextManager> textActorToSpawn;


	
public:
	// ===========================================
	// Ciclo de Vida:
	// ===========================================
	ULR_EntityTextInfoComponent();
	virtual void BeginPlay() override;

	

	// ===========================================
	// Metodos de spawn de Texto:
	// ===========================================
	UFUNCTION(BlueprintCallable, Category="Damage Numbers")
	void SpawnDamageNumber(FTransform SpawnTransform, ELRTextContentAnimationDirection animationDirection);
};

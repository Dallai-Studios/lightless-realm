#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LR_EntityGameTextManager.generated.h"

UCLASS()
class LIGHTLESSREALM_API ALR_EntityGameTextManager : public AActor {
	GENERATED_BODY()

public:
	// ===========================================
	// Ciclo de Vida:
	// ===========================================
	ALR_EntityGameTextManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

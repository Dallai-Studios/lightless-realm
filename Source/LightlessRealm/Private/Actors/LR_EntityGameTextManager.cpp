#include "Actors/LR_EntityGameTextManager.h"



// ===========================================
// Ciclo de Vida:
// ===========================================
ALR_EntityGameTextManager::ALR_EntityGameTextManager() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALR_EntityGameTextManager::BeginPlay() {
	Super::BeginPlay();
}

void ALR_EntityGameTextManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

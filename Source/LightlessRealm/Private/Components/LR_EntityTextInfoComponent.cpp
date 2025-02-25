#include "Components/LR_EntityTextInfoComponent.h"
#include "Actors/LR_EntityGameTextManager.h"


// ===========================================
// Ciclo de Vida:
// ===========================================
ULR_EntityTextInfoComponent::ULR_EntityTextInfoComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void ULR_EntityTextInfoComponent::BeginPlay() {
	Super::BeginPlay();
}

void ULR_EntityTextInfoComponent::SpawnDamageNumber(FTransform spawnTransform, ELRTextContentAnimationDirection animationDirection) {
	FActorSpawnParameters params;
	params.Owner = this->GetOwner();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto spawnedTextManager = this->GetWorld()->SpawnActor<ALR_EntityGameTextManager>(this->textActorToSpawn, spawnTransform, params);

	spawnedTextManager->AnimateTextContent(animationDirection);
}
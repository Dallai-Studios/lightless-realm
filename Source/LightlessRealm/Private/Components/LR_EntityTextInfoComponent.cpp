#include "Components/LR_EntityTextInfoComponent.h"


// ===========================================
// Ciclo de Vida:
// ===========================================
ULR_EntityTextInfoComponent::ULR_EntityTextInfoComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void ULR_EntityTextInfoComponent::BeginPlay() {
	Super::BeginPlay();
}
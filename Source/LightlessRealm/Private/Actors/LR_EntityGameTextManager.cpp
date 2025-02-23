#include "Actors/LR_EntityGameTextManager.h"

// ===========================================
// Ciclo de Vida:
// ===========================================
ALR_EntityGameTextManager::ALR_EntityGameTextManager() {
	PrimaryActorTick.bCanEverTick = true;

	// this->infoWidgetComponent = this->CreateDefaultSubobject<UWidgetComponent>("Text Widget Component");
	// this->infoWidgetComponent->SetupAttachment(this->GetRootComponent());
}

void ALR_EntityGameTextManager::BeginPlay() {
	Super::BeginPlay();

	// if (this->infoWidgetComponent->GetWidget() != NULL) {
	// 	// this->hudReference = Cast<ULR_EntityTextDisplayHud>(this->infoWidgetComponent->GetWidget());
	// }
}

void ALR_EntityGameTextManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
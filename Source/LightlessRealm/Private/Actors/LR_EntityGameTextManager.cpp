#include "Actors/LR_EntityGameTextManager.h"

#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/LR_EntityTextHud.h"
#include "Tools/LR_Utils.h"

// ===========================================
// Ciclo de Vida:
// ===========================================
ALR_EntityGameTextManager::ALR_EntityGameTextManager() {
	PrimaryActorTick.bCanEverTick = false;

	this->widgetComponent = this->CreateDefaultSubobject<UWidgetComponent>("Text Widget Component");
	this->widgetComponent->SetupAttachment(this->GetRootComponent());

	this->animationTimelineComponent = this->CreateDefaultSubobject<UTimelineComponent>("Animation Timeline Component");
}

void ALR_EntityGameTextManager::BeginPlay() {
	Super::BeginPlay();

	if (this->widgetComponent->GetWidget() != NULL) {
		this->widgetInstance = Cast<ULR_EntityTextHud>(this->widgetComponent->GetWidget());
	} else {
		ULR_Utils::ShowDebugMessage(TEXT("Entity Text HUD is not defined on the Entity Game Text Manager"));
	}
}

void ALR_EntityGameTextManager::AnimateTextContent(ELRTextContentAnimationDirection animationDirection) {
	FOnTimelineVector timelineCallback;
	timelineCallback.BindUFunction(this, "UpdateTextAnimationPosition");

	FOnTimelineVector timelineFinishCallback;
	
	switch (animationDirection) {
		case ELRTextContentAnimationDirection::ANIMTE_UP: this->animationTimelineComponent->AddInterpVector(this->animationUpCurve, timelineCallback); break;
		case ELRTextContentAnimationDirection::ANIMATE_DOWN: this->animationTimelineComponent->AddInterpVector(this->animationDownCurve, timelineCallback); break;
		case ELRTextContentAnimationDirection::ANIMTE_LEFT: this->animationTimelineComponent->AddInterpVector(this->animationLeftCurve, timelineCallback); break;
		case ELRTextContentAnimationDirection::ANIMATE_RIGHT: this->animationTimelineComponent->AddInterpVector(this->animationRightCurve, timelineCallback); break;
	}

	this->animationTimelineComponent->PlayFromStart();
}

void ALR_EntityGameTextManager::UpdateTextAnimationPosition(FVector value) {
	this->widgetComponent->SetRelativeLocation(value);
}

void ALR_EntityGameTextManager::FinishTextAnimationPosition() {
	this->widgetComponent->SetRelativeLocation(FVector(0, 0, 0));
}
#pragma once

#include "CoreMinimal.h"
#include "Enums/ELRTextContentAnimationDirection.h"
#include "GameFramework/Actor.h"
#include "LR_EntityGameTextManager.generated.h"

UCLASS()
class LIGHTLESSREALM_API ALR_EntityGameTextManager : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UWidgetComponent> widgetComponent;
	class ULR_EntityTextHud* widgetInstance;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UTimelineComponent> animationTimelineComponent;

	UPROPERTY(EditAnywhere, Category="Animations")
	class UCurveVector* animationUpCurve;
	
	UPROPERTY(EditAnywhere, Category="Animations")
	UCurveVector* animationDownCurve;

	UPROPERTY(EditAnywhere, Category="Animations")
	UCurveVector* animationLeftCurve;

	UPROPERTY(EditAnywhere, Category="Animations")
	UCurveVector* animationRightCurve;

	
public:
	// ===========================================
	// Ciclo de Vida:
	// ===========================================
	ALR_EntityGameTextManager();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Animation")
	void AnimateTextContent(ELRTextContentAnimationDirection animationDirection);
};

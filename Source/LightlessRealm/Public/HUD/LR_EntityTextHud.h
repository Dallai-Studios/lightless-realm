#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LR_EntityTextHud.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_EntityTextHud : public UUserWidget {
	GENERATED_BODY()

private:
	FText textContent;

public:
	UFUNCTION(BlueprintCallable, Category="Functions")
	void SetTextContent(FText content);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Functions")
	FText GetTextContent() const;
};

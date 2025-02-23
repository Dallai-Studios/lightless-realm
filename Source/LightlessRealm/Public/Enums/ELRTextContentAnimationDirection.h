#pragma once

#include "CoreMinimal.h"
#include "ELRTextContentAnimationDirection.generated.h"

UENUM(BlueprintType)
enum ELRTextContentAnimationDirection : uint8 {
	ANIMTE_UP,
	ANIMATE_DOWN,
	ANIMTE_LEFT,
	ANIMATE_RIGHT,
};

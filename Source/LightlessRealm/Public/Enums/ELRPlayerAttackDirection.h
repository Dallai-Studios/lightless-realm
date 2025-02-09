#pragma once

#include "CoreMinimal.h"
#include "ELRPlayerMovementDirection.generated.h"

UENUM(BlueprintType)
enum ElrPlayerAttackDirection : uint8 {
	ATTACK_UP,
	ATTACK_DOWN,
	ATTACK_LEFT,
	ATTACK_RIGHT,
};

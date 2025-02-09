#pragma once

#include "CoreMinimal.h"
#include "ELRPlayerAttackDirection.generated.h"

UENUM(BlueprintType)
enum ELRPlayerAttackDirection : uint8 {
	ATTACK_UP,
	ATTACK_DOWN,
	ATTACK_LEFT,
	ATTACK_RIGHT,
};

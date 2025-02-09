#pragma once

#include "CoreMinimal.h"
#include "ELRPlayerMovementDirection.generated.h"

UENUM()
enum ELRPlayerMovementDirection : uint8 {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
};

﻿#pragma once

#include "CoreMinimal.h"
#include "ELRPlayerMovementDirection.generated.h"

UENUM(BlueprintType)
enum ELRPlayerMovementDirection : uint8 {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
};

#pragma once

#include "CoreMinimal.h"
#include "ELREnemyType.generated.h"

UENUM(BlueprintType)
enum class ELREnemyType : uint8 {
	ANIMAL,
	BEAST,
	HUMANOID,
	DEMONIC,
	MAGIC,
	UNDEAD,
};
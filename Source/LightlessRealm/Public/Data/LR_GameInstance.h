
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LR_GameInstance.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_GameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Game")
	class ULR_PlayerCharacterPDA* gameSelectedCharacter;
};

#include "Tools/LR_Utils.h"

void ULR_Utils::ShowDebugMessage(FString message, FColor messageColor) {
	GEngine->AddOnScreenDebugMessage(-1, 100, messageColor, message, false);
}

#include "HUD/LR_EntityTextHud.h"
void ULR_EntityTextHud::SetTextContent(FText content) {
	this->textContent = content;
}

FText ULR_EntityTextHud::GetTextContent() const {
	return this->textContent;
}
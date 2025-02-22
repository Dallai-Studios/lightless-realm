#include "Components/LR_EntityAttributesComponent.h"

#include "Data/LR_EnemyPDA.h"
#include "Data/LR_PlayerCharacterPDA.h"

ULR_EntityAttributesComponent::ULR_EntityAttributesComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void ULR_EntityAttributesComponent::BeginPlay() {
	Super::BeginPlay();
}

void ULR_EntityAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULR_EntityAttributesComponent::BuildAttributesFromCharacter(ULR_PlayerCharacterPDA* character) {
	this->health = character->health;
	this->maxHealth = character->maxHealth;
	this->magika = character->magika;
	this->maxMagika = character->maxMagika;
}

void ULR_EntityAttributesComponent::BuildAttributesFromEnemy(ULR_EnemyPDA* enemy) {
	this->health = enemy->health;
	this->maxHealth = enemy->maxHealth;
	this->magika = enemy->magika;
	this->maxMagika = enemy->maxMagika;
}

void ULR_EntityAttributesComponent::SetNewMaxHealthAmount(float maxHealthAmount) {
	if (maxHealthAmount <= 0 || this->isDead) return;
	this->maxHealth = maxHealthAmount;
	if (this->health > this->maxHealth) this->health = this->maxHealth;
}

void ULR_EntityAttributesComponent::AddHealth(float healthAmount) {
	if (this->isDead) return;
	this->health += healthAmount;
	if (this->health > this->maxHealth) this->health = this->maxHealth;
}

void ULR_EntityAttributesComponent::RemoveHealth(float amount) {
	if (this->isDead) return;

	this->health -= amount;

	if (this->health <= 0) {
		this->health = 0;
		this->isDead = true;
		this->OnEntityDie.Broadcast();
	} 
}

void ULR_EntityAttributesComponent::SetNewMaxMagikaAmount(float maxMagikaAmount) {
	if (maxMagikaAmount <= 0 || this->isDead) return;
	this->maxMagika = maxMagikaAmount;
	if (this->magika > this->maxMagika) this->magika = this->maxMagika;
}

void ULR_EntityAttributesComponent::AddMagika(float magikaAmount) {
	if (this->isDead) return;
	this->magika += magikaAmount;
	if (this->magika > this->maxMagika) this->magika = this->maxMagika;
}

bool ULR_EntityAttributesComponent::HasEnoughMagikaToPerformAction(const float amount) {
	return this->magika >= amount;
}

void ULR_EntityAttributesComponent::RemoveMagika(float magikaAmount) {
	if (this->isDead || this->magika < magikaAmount) return;
	this->magika -= magikaAmount;
}

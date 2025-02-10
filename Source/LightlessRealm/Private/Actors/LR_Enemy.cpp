#include "Actors/LR_Enemy.h"

#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/LR_EnemyPDA.h"
#include "Data/LR_GameEventsPDA.h"


ALR_Enemy::ALR_Enemy() {
	PrimaryActorTick.bCanEverTick = true;

	this->collision = this->CreateDefaultSubobject<UCapsuleComponent>("Collision Capsule");
	this->RootComponent = this->collision;

	this->flipbookComponent = this->CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook Component");
	this->flipbookComponent->SetupAttachment(this->collision);

	if (this->enemyConfig) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}
}

void ALR_Enemy::BeginPlay() {
	Super::BeginPlay();
	if (this->gameEvents) this->gameEvents->OnPlayerPerformAction.AddDynamic(this, &ALR_Enemy::ALR_Enemy::RespondToPlayerAction);
}

void ALR_Enemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->MoveEnemy(DeltaTime);
}

void ALR_Enemy::MoveUp() {
	if (!this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_UP)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_UP;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + this->movementSize, currentLocation.Y, currentLocation.Z);
}

void ALR_Enemy::MoveDown() {
	if (!this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_DOWN)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_DOWN;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + (this->movementSize * -1), currentLocation.Y, currentLocation.Z);
}

void ALR_Enemy::MoveLeft() {
	if (!this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_LEFT)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_LEFT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + (this->movementSize * -1), currentLocation.Z);

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
}

void ALR_Enemy::MoveRight() {
	if (!this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_RIGHT)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_RIGHT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + this->movementSize, currentLocation.Z);

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X > 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
}

void ALR_Enemy::RespondToPlayerAction() {
	/**
	 * todo: 1 - Escolher randomicamente uma direção para se mover fazendo um random entre 1 e 4
	 * todo: caso 1-UP 2-DOWN 3-LEFT 4-RIGHT
	 * todo: 2 - Verificar se pode ir pra direção escolhida, se não puder ja não faz mas nada
	 * todo: 3 - Adiciona o movimento pra direção escolhida, a movimentação será cuidada pelo update
	 * todo: 4 - verifica se o player está no raio de visão, caso esteja, adiciona como target
	 */
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Listened to player action"));
}

void ALR_Enemy::MoveEnemy(float deltaTime) {
	if (this->destinationLocation == this->GetActorLocation()) return;
	FVector newLocation = FMath::LerpStable(this->GetActorLocation(), this->destinationLocation, deltaTime * this->movementSpeed);
	this->SetActorLocation(newLocation);
}

bool ALR_Enemy::CheckForPathBlock(ELRPlayerMovementDirection direction) {
	auto lineStart = this->GetActorLocation();
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	
	FVector lineEnd;
	
	if (direction == ELRPlayerMovementDirection::DIRECTION_UP) lineEnd = lineStart + (this->GetActorForwardVector() * this->movementSize);
	if (direction == ELRPlayerMovementDirection::DIRECTION_DOWN) lineEnd = lineStart + (this->GetActorForwardVector() * this->movementSize * -1);
	if (direction == ELRPlayerMovementDirection::DIRECTION_RIGHT) lineEnd = lineStart + (this->GetActorRightVector() * this->movementSize);
	if (direction == ELRPlayerMovementDirection::DIRECTION_LEFT) lineEnd = lineStart + (this->GetActorRightVector() * this->movementSize * -1);
	
	this->GetWorld()->LineTraceSingleByChannel(hitResult, lineStart, lineEnd, ECC_Visibility, params);
	
	DrawDebugLine(this->GetWorld(), lineStart, lineEnd, hitResult.IsValidBlockingHit() ? FColor::Green : FColor::Red, false, 10);
	
	return hitResult.IsValidBlockingHit();
}
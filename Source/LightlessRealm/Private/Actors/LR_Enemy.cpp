#include "Actors/LR_Enemy.h"
#include "PaperFlipbookComponent.h"
#include "Characters/LR_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Data/LR_EnemyPDA.h"
#include "Data/LR_GameEventsPDA.h"
#include "Data/LR_GameInstance.h"
#include "Data/LR_PlayerCharacterPDA.h"


// lifecycles:
ALR_Enemy::ALR_Enemy() {
	PrimaryActorTick.bCanEverTick = true;

	this->collision = this->CreateDefaultSubobject<UCapsuleComponent>("Collision Capsule");
	this->RootComponent = this->collision;

	this->flipbookComponent = this->CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook Component");
	this->flipbookComponent->SetupAttachment(this->collision);

	this->playerDetectionSphere = this->CreateDefaultSubobject<USphereComponent>("Player Detection Sphere");
	
	if (this->enemyConfig != nullptr) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}
}


void ALR_Enemy::BeginPlay() {
	Super::BeginPlay();

	// essa configuração serve só pra ler todas as propriedades do data asset que representa esse inimigo
	// nenhuma configuração é feita para validar o personagem selecionado pelo jogador. -Renan
	this->Configure();

	if (this->gameEvents) this->gameEvents->OnPlayerPerformAction.AddDynamic(this, &ALR_Enemy::ALR_Enemy::RespondToPlayerAction);

	this->destinationLocation = this->GetActorLocation();

	ULR_GameInstance* gameInstance = Cast<ULR_GameInstance>(this->GetWorld()->GetGameInstance());
	if (gameInstance->gameSelectedCharacter) this->SetupEnemyBasedOnSelectedCharacter(gameInstance);

	this->playerDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ALR_Enemy::ALR_Enemy::CheckForTarget);
}


void ALR_Enemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->MoveEnemy(DeltaTime);
}


void ALR_Enemy::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	this->Destroy();
}



// common:
void ALR_Enemy::MoveUp() {
	if (!this->canMoveUp) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_UP)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_UP;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + this->movementSize, currentLocation.Y, currentLocation.Z);
}


void ALR_Enemy::MoveDown() {
	if (!this->canMoveDown) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_DOWN)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_DOWN;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + (this->movementSize * -1), currentLocation.Y, currentLocation.Z);
}


void ALR_Enemy::MoveLeft() {
	if (!this->canMoveLeft) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_LEFT)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_LEFT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + (this->movementSize * -1), currentLocation.Z);

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
}


void ALR_Enemy::MoveRight() {
	if (!this->canMoveRight) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_RIGHT)) return;
	
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
	 * todo: caso 1-UP 2-DOWN 3-LEFT 4-RIGHT 0 e 5-STAY
	 * todo: 2 - Verificar se pode ir pra direção escolhida, se não puder ja não faz mas nada
	 * todo: 3 - Adiciona o movimento pra direção escolhida, a movimentação será cuidada pelo update
	 * todo: 4 - verifica se o player está no raio de visão, caso esteja, adiciona como target
	 */
	

	// caso tenho um target ativo, a logica é bem mais complicada
	if (IsValid(this->activeTarget)) {
		return;
	}
	
	auto randomDirection = FMath::RandRange(0, 5);
	if (randomDirection == 1) this->MoveUp();
	if (randomDirection == 2) this->MoveDown();
	if (randomDirection == 3) this->MoveLeft();
	if (randomDirection == 4) this->MoveRight();
	if (randomDirection == 0 || randomDirection == 5) return;
}


void ALR_Enemy::MoveEnemy(float deltaTime) {
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


void ALR_Enemy::Configure() {
	if (this->enemyConfig) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}
}


void ALR_Enemy::SetupEnemyBasedOnSelectedCharacter(ULR_GameInstance* gameInstance) {
	check(gameInstance);
	check(gameInstance->gameSelectedCharacter);
	
	if (gameInstance->gameSelectedCharacter->easyToDetect) {
		this->playerDetectionSphere->SetSphereRadius(this->playerDetectionSphere->GetScaledSphereRadius() * 2);
	}

	if (gameInstance->gameSelectedCharacter->hardToDetectOnShadows) {
		this->playerDetectionSphere->SetSphereRadius(this->playerDetectionSphere->GetScaledSphereRadius() / 2);
	}
}


void ALR_Enemy::CheckForTarget(
	UPrimitiveComponent* overlapedComponent, 
	AActor* otherActor, 
	UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex,
	bool fromSweep,
	const FHitResult& SweepResult
) {
	ALR_PlayerCharacter* player = Cast<ALR_PlayerCharacter>(otherActor);
	if (IsValid(player)) {
		
	}
}
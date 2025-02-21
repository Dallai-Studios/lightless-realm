#include "Components/LR_FootstepAudioComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Tools/LR_Utils.h"


// ======================================================
// Ciclo de vida:
// ======================================================
ULR_FootstepAudioComponent::ULR_FootstepAudioComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void ULR_FootstepAudioComponent::BeginPlay() {
	Super::BeginPlay();
}



// ======================================================
// Metodos de som de passos:
// ======================================================
void ULR_FootstepAudioComponent::PlayFootstepSoundAtActorLocation() {
	FHitResult groundHit;

	FCollisionQueryParams queryParams;
	queryParams.bReturnPhysicalMaterial = true;
	queryParams.AddIgnoredActor(this->GetOwner());
	
	auto startVector = this->GetOwner()->GetActorLocation();
	auto endVector = startVector + (this->GetOwner()->GetActorUpVector() * this->floorDetectionRaySize * -1);
	
	this->GetWorld()->LineTraceSingleByChannel(groundHit, startVector, endVector, ECC_Visibility, queryParams);

	DrawDebugLine(this->GetWorld(), startVector, endVector, groundHit.bBlockingHit ? FColor::Emerald : FColor::Cyan, false, 20);

	if (!groundHit.bBlockingHit) return;
	
	switch(groundHit.PhysMaterial->SurfaceType) {
		case SurfaceType1: // acho que isso aqui é grass.
			UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), this->grassFootstepSounds, this->GetOwner()->GetActorLocation(), 1, 1,0, this->attenuationSettings);
			break;
		default:
			UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), this->grassFootstepSounds, this->GetOwner()->GetActorLocation(), 1, 1,0, this->attenuationSettings);
			break;
	}
}
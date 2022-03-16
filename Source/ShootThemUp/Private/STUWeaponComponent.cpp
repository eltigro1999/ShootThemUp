// Fill out your copyright notice in the Description page of Project Settings.


#include "STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon() {
	if (!GetWorld()) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
	if (!CurrentWeapon) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);

	CurrentWeapon->SetOwner(Character);
}

void USTUWeaponComponent::StartFire() {
	if (!CurrentWeapon) return;
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire() {
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}
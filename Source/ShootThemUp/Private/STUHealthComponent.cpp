// Fill out your copyright notice in the Description page of Project Settings.


#include "STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(DamageTypeLog, All, All);

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner) {
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser) {
	if (IsDead() || Damage <= 0.0f || !GetWorld()) return;
	SetHealth(Health-Damage);
	if (IsDead()) {
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
		OnDeath.Broadcast();
	}
	else if (AutoHeal) {
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
	
}

void USTUHealthComponent::HealUpdate() {
	SetHealth(Health + HealModifier);
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld()) {
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float UpdateHealth) {
	Health = FMath::Clamp(UpdateHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
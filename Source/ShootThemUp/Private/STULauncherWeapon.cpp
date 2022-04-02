// Fill out your copyright notice in the Description page of Project Settings.


#include "STULauncherWeapon.h"
#include "STUProjectile.h"
#include "Kismet/GameplayStatics.h"


void ASTULauncherWeapon::StartFire() {
	MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
	//
	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
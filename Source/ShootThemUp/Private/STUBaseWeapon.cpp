// Fill out your copyright notice in the Description page of Project Settings.


#include "STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "STUBaseCharacter.h"
#include "STUPlayerController.h"


// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTUBaseWeapon::Fire() {
	MakeShot();
}

void ASTUBaseWeapon::MakeShot() {
	if (!GetWorld()) return;	
	FVector TraceStart;
	FVector TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd)) return;
	FHitResult HitResult;
	MakeHit(TraceStart, TraceEnd, HitResult);
	if (HitResult.bBlockingHit) {
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24 ,FColor::Red, false, 5.0f);
		
	} else {
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
}

APlayerController* ASTUBaseWeapon::GetPlayerController(){
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;
	return Player->GetController<APlayerController>();
} 

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) {
	const auto Controller = GetPlayerController();
	if (!Controller) return false;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const {
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) {
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
	TraceStart = ViewLocation; 
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult) {
	if (!GetWorld()) return;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult) {
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;
	DamagedActor->TakeDamage(Damage, FDamageEvent(), GetPlayerController(), this);
}
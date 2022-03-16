// Fill out your copyright notice in the Description page of Project Settings.


#include "STUBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "STUCharacterMovementComponent.h"
#include "STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.h"
#include "STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);
DEFINE_LOG_CATEGORY_STATIC(OnDeathLog, All, All);


// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit):
	Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera component");
	CameraComponent->SetupAttachment(SpringArm);
	
	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("Health component");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("Health value text");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);//Check is a macro that checks if the object is null
	check(HealthTextComponent);
	check(GetCharacterMovement());

	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
	
}



// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	check(WeaponComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(FName("TurnAround"), this, &ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Run"), IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction(FName("Run"), IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction(FName("Fire"),IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);
}

void ASTUBaseCharacter::MoveForward(float Amount) {
	if (Amount!=0.0f) {
		isMovingForward = Amount > 0.0f;
		AddMovementInput(GetActorForwardVector(), Amount);
	}
}
void ASTUBaseCharacter::MoveRight(float Amount) {
	if (Amount!=0.0f) {
		AddMovementInput(GetActorRightVector(), Amount);
	}
}

void ASTUBaseCharacter::OnStartRunning() {
	wannaRun = true;
}

void ASTUBaseCharacter::OnStopRunning() {
	wannaRun = false;
}

bool ASTUBaseCharacter::IsRunning() const {
	return wannaRun && isMovingForward && !GetVelocity().IsZero();
} 

float ASTUBaseCharacter::GetMovementDirection() const {
	if (GetVelocity().IsZero()) return 0.0f;
	const FVector VelocityNormalised = GetVelocity().GetSafeNormal();
	const float angle_between_velocity_and_forward_vector = FMath::Acos(FVector::DotProduct(VelocityNormalised, GetActorForwardVector()));
	const FVector velocity_and_forward_vector_cross_product =
		FVector::CrossProduct(GetActorForwardVector(), VelocityNormalised);
	return FMath::RadiansToDegrees(FMath::Sign(velocity_and_forward_vector_cross_product.Z)*angle_between_velocity_and_forward_vector);
}

void ASTUBaseCharacter::OnDeath() {
	UE_LOG(OnDeathLog, Display, TEXT("Player %s is dead"), *GetName());
	PlayAnimMontage(DeathAnimMontage);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);
	if (Controller) Controller->ChangeState(NAME_Spectating);
}

void ASTUBaseCharacter::OnHealthChanged(float Health) {
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit) {
	const float FallVelocityZ = -GetVelocity().Z;
	if (FallVelocityZ < LandedDamageVelocity.X) return;
	const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
}


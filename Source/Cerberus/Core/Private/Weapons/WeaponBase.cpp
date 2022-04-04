// Fill out your copyright notice in the Description page of Project Settings.


#include "Cerberus/Core/Public/Weapons/WeaponBase.h"
#include "Cerberus/Core/Public/Combat/AttackComponent.h"
#include "Cerberus/Dev/Public/Projectile.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->ReplicatedAttack.BindUObject(this, &AWeaponBase::ServerOnAttack);
	AddOwnedComponent(AttackComponent);

	//Hopefully get the player/owner of the weapon

}

void AWeaponBase::SetController(AController* Controller)
{
	Player = Controller;

	if (GetInstigator())
	{
		Player  = GetInstigator()->GetController();
	}
		
	if (GEngine && Player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Player->GetHumanReadableName());
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void AWeaponBase::ServerOnAttack_Implementation()
{
	//Spawning selected projectile via the attack component
	if (bIsProjectileWeapon)
	{
		FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
		FRotator spawnRotation = GetActorRotation();

		FActorSpawnParameters spawnParameters;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.Owner = this;

		
		//AttackComponent->FireProjectile<AProjectile>(spawnLocation, spawnRotation, spawnParameters);
	}
	else
	{
		
	}

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::TraceForward()
{
	FVector loc;
	FRotator rot;
	FHitResult hit;

	Player->GetPlayerViewPoint(loc, rot);

	FVector start = loc;
	FVector end = start + (rot.Vector() * Range);

	FCollisionQueryParams traceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, traceParams);

	DrawDebugLine(GetWorld(), start, end, FColor::Cyan, false, 2.0f);
	
	if (bHit)
	{
		DrawDebugBox(GetWorld(), hit.ImpactPoint, FVector(5,5,5), FColor::Red, false, 2.0f);

		AActor* Interactable = hit.GetActor();

		
		if (Interactable && Interactable != FocusedActor)
		{
			FocusedActor = Interactable;
		} else
		{
			FocusedActor = nullptr;
		}

		//Preform damage
		if (FocusedActor == Interactable)
		{
			
		}
	}
}


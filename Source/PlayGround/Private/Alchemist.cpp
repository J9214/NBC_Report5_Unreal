// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist.h"

// Sets default values
AAlchemist::AAlchemist()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlchemist::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlchemist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


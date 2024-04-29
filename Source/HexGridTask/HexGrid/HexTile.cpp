// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGrid/HexTile.h"

// Sets default values
AHexTile::AHexTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	HexMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HexMesh"));
	HexMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHexTile::BeginPlay()
{
	Super::BeginPlay();
}

FHexCoordinates AHexTile::GetCoordinates() const
{
	return HexCoordinates;
}

void AHexTile::InitializeHex(FHexCoordinates NewHexCoordinates)
{
	HexCoordinates = NewHexCoordinates;
}
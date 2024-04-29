// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGrid/HexGridManager.h"
#include "Kismet/KismetSystemLibrary.h"

AHexGridManager::AHexGridManager()
{
}

void AHexGridManager::InitRadialHexGrid()
{
	int32 TotalTiles = 3 * GridRadius * (GridRadius + 1);
	TileMap.Reserve(TotalTiles + 1);

	if (bUseUniformOffset)
	{
		XOffset = 1;
		YOffset = UniformOffsetRatio;
	}

    XOffset *= OffsetMultiplier;
    YOffset *= OffsetMultiplier;

    int32 index = 0;
    for (int32 q = -GridRadius; q <= GridRadius; q++)
    {
        int32 r1 = FMath::Max(-GridRadius, -q - GridRadius);
        int32 r2 = FMath::Min(GridRadius, -q + GridRadius);
        for (int32 r = r1; r <= r2; r++)
        {
			// Offset the tile location
			const float LocationX = XOffset * q;
			const float LocationY = YOffset * r + (YOffset / 2) * q;

			// Convert the 2D grid coordinates to hexagonal coordinates
			FHexCoordinates HexCoordinates;
			HexCoordinates.Q = q;
			HexCoordinates.R = r;
			HexCoordinates.CalculateS();

			SpawnTile(LocationX, LocationY, HexCoordinates, q, r);
		}
    }
}

void AHexGridManager::InitRectangularHexGrid()
{
	TileMap.Reserve(GridWidth * GridHeight);

	if (bUseUniformOffset)
	{
		XOffset = UniformOffsetRatio;
		YOffset = 1;
	}

	XOffset *= OffsetMultiplier;
	YOffset *= OffsetMultiplier;

	for (int32 i = 0; i < GridWidth * GridHeight; i++)
	{
		int32 x = i % GridWidth;
		int32 y = i / GridWidth;
		const bool oddRow = y % 2 == 1;

		// Offset the tile location
		const float LocationX = oddRow ? (x * XOffset) + (XOffset / 2) : x * XOffset;
		const float LocationY = y * YOffset;

		// Offset the x coordinate for correct calculations
		x = oddRow ? (x * 2) + 1 : x * 2;
		y /= 2;

		// Convert the 2D grid coordinates to hexagonal coordinates
		FHexCoordinates HexCoordinates;
		HexCoordinates.Q = x;
		HexCoordinates.R = y - (x - (x & 1)) / 2;
		HexCoordinates.CalculateS();

		SpawnTile(LocationX, LocationY, HexCoordinates, x, y);
	}
}

void AHexGridManager::SpawnTile(const float LocationX, const float LocationY, FHexCoordinates HexCoordinates, int32 x, int32 y)
{
	FVector Location = FVector(GetActorLocation().X + LocationX, GetActorLocation().Y + LocationY, 0.f);
	FTransform Transform = FTransform(GetActorRotation(), Location);

	AHexTile* Tile = GetWorld()->SpawnActorDeferred<AHexTile>(HexTileClass, Transform);
	if (!IsValid(Tile))
	{
		return;
	}

	Tile->InitializeHex(HexCoordinates);
	TileMap.Add(HexCoordinates, Tile);

	Tile->FinishSpawning(Transform);

	if (bShowXYCoordinates)
	{
		UKismetSystemLibrary::DrawDebugString(this, FVector::ZAxisVector * 20, FString::Printf(TEXT("(%d,%d)"), x, y), Tile, FLinearColor::Black, 100);
	}
}

TArray<AHexTile*> AHexGridManager::GetNeighbors(AHexTile* Tile)
{
	TArray<AHexTile*> Neighbors;

	if (!IsValid(Tile))
	{
		return Neighbors;
	}

	FHexCoordinates TileCoordinates = Tile->HexCoordinates;

	for (FVector Direction : Directions)
	{
		FHexCoordinates NewTileCoordinates;
		NewTileCoordinates.Q = TileCoordinates.Q + Direction.X;
		NewTileCoordinates.R = TileCoordinates.R + Direction.Y;
		NewTileCoordinates.S = TileCoordinates.S + Direction.Z;

		if (AHexTile* NeighborTile = GetTile(NewTileCoordinates))
		{
			Neighbors.Add(NeighborTile);
			UKismetSystemLibrary::DrawDebugString(this, FVector::ZAxisVector * 20, TEXT("Found neighbor!"), NeighborTile, FLinearColor::Black, 10);
		}
	}

	return Neighbors;
}

AHexTile* AHexGridManager::GetTile(FHexCoordinates HexCoordinates)
{	
	AHexTile** TilePtr = TileMap.Find(HexCoordinates);
	
	if (!TilePtr)
	{
		return nullptr;
	}

	return *TilePtr;
}

int32 AHexGridManager::Distance(FHexCoordinates Hex1, FHexCoordinates Hex2)
{
	return (FMath::Abs(Hex1.Q - Hex2.Q) + FMath::Abs(Hex1.R - Hex2.R) + FMath::Abs(Hex1.S - Hex2.S)) / 2;
}

void AHexGridManager::PrintNeighbors()
{
	FHexCoordinates DebugCoordinates;
	DebugCoordinates.Q = DebugCoordinateQ1;
	DebugCoordinates.R = DebugCoordinateR1;
	DebugCoordinates.CalculateS();

	if (AHexTile* Tile = GetTile(DebugCoordinates))
	{
		GetNeighbors(Tile);
	}
}

void AHexGridManager::PrintDistance()
{
	FHexCoordinates Hex1;
	Hex1.Q = DebugCoordinateQ1;
	Hex1.R = DebugCoordinateR1;
	Hex1.CalculateS();

	FHexCoordinates Hex2;
	Hex2.Q = DebugCoordinateQ2;
	Hex2.R = DebugCoordinateR2;
	Hex2.CalculateS();

	FString Hex1Str = Hex1.ToString();
	FString Hex2Str = Hex2.ToString();
	int32 Dist = Distance(Hex1, Hex2);

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Distance between %s and %s is %d"), *Hex1Str, *Hex2Str, Dist), true, true, FLinearColor::Green, 10);
}


// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();
	
	switch (HexGridType)
	{
		case Rectangular:
		{
			InitRectangularHexGrid();
			break;
		}
		case Radial:
		{
			InitRadialHexGrid();
			break;
		}
	}
}

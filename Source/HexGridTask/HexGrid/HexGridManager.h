// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "HexGridManager.generated.h"

UENUM()
enum EHexGridType
{
	Rectangular,
	Radial
};


UCLASS()
class HEXGRIDTASK_API AHexGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHexGridManager();

	// General grid settings

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings")
	TEnumAsByte<EHexGridType> HexGridType;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings")
	TSubclassOf<AHexTile> HexTileClass;

	// Grid size settings

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Radial Grid")
	int32 GridRadius = 1;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Rectangular Grid")
	int32 GridWidth = 2;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Rectangular Grid")
	int32 GridHeight = 2;

	// Tile offset properties

	// Makes the X and Y offset be a pre-defined ratio which will be multiplied uniformally by UniformOffsetRatio
	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Offset")
	bool bUseUniformOffset = false;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Offset")
	float OffsetMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Offset|Manual")
	float XOffset = 90.f;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Offset|Manual")
	float YOffset = 26.f;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Settings|Offset|Uniform")
	float UniformOffsetRatio = 3.47;

	// Debug properties 

	UPROPERTY(EditAnywhere, Category = "HexGrid|Debug")
	bool bShowXYCoordinates = false;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Debug")
	int32 DebugCoordinateQ1;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Debug")
	int32 DebugCoordinateR1;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Debug")
	int32 DebugCoordinateQ2;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Debug")
	int32 DebugCoordinateR2;


	UFUNCTION(BlueprintCallable, Category = "HexGrid")
	TArray<AHexTile*> GetNeighbors(AHexTile* Tile);

	UFUNCTION(BlueprintPure, Category = "HexGrid")
	int32 Distance(FHexCoordinates Hex1, FHexCoordinates Hex2);

	UFUNCTION(BlueprintCallable, Category = "HexGrid|Settings")
	AHexTile* GetTile(FHexCoordinates HexCoordinates);

	// Print neighbors of DebugCoordinates1
	UFUNCTION(Category = "HexGrid|Debug", CallInEditor)
	void PrintNeighbors();

	// Print distance between DebugCoordinates1 and DebugCoordinates2
	UFUNCTION(Category = "HexGrid|Debug", CallInEditor)
	void PrintDistance();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TMap<FHexCoordinates, AHexTile*> TileMap;

	void InitRadialHexGrid();

	void InitRectangularHexGrid();

	void SpawnTile(const float LocationX, const float LocationY, FHexCoordinates HexCoordinates, int32 x, int32 y);

	// Define the directions for the 6 neighbors
	TArray<FVector> Directions = {
	FVector(1, 0, -1), FVector(+1, -1, 0), FVector(0, -1, 1),
	FVector(-1, 0, 1), FVector(-1, 1, 0), FVector(0, 1, -1)
	};
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/UnrealString.h" 
#include "GenericPlatform/GenericPlatformMath.h"
#include "HexTile.generated.h"

USTRUCT(BlueprintType)
struct FHexCoordinates
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 Q;

	UPROPERTY(BlueprintReadWrite)
	int32 R;

	UPROPERTY(BlueprintReadWrite)
	int32 S;

	void CalculateS()
	{
		S = -Q - R;
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("Q:%d, R:%d, S:%d"), Q, R, S);
	}

	bool operator==(const FHexCoordinates& Other) const
	{
		return (Q == Other.Q && R == Other.R && S == Other.S);
	}

	friend uint32 GetTypeHash(const FHexCoordinates& Hex)
	{
		return HashCombine(GetTypeHash(Hex.Q), HashCombine(GetTypeHash(Hex.R), GetTypeHash(Hex.S)));
	}
};

UCLASS()
class HEXGRIDTASK_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexTile();

	UPROPERTY(EditAnywhere, Category = "HexTile|Visuals")
	UStaticMeshComponent* HexMesh;

	UFUNCTION(BlueprintCallable, Category = "HexGrid")
	FHexCoordinates GetCoordinates() const;

	virtual void InitializeHex(FHexCoordinates NewHexCoordinates);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "HexTile|Coordinates")
	FHexCoordinates HexCoordinates;

};

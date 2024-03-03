// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

//Numerativo per dirmi se la tile è libera o occupata
UENUM()
enum class ETileStatus : uint8
{
	EMPTY     UMETA(DisplayName = "Empty"),
	OCCUPIED      UMETA(DisplayName = "Occupied"),
};



UCLASS()
class CHESS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// SET player owner e status 
	void SetTileStatus(const int32 TileOwner, const ETileStatus TileStatus);

	// GET lo stato della tile (se c'è una pedina)
	ETileStatus GetTileStatus();

	// Tile appartiene a 0 o 1
	int32 GetOwner();

	//SET posizione x,y della tile
	void SetGridPosition(const double InX, const double InY);

	//GET posizione x,y della tile
	FVector2D GetGridPosition();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Per essere messo nell'editor 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	//Per poter aggiungere il materiale
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;


	//Status e PlayerOwner devono poter essere viste da tutti
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETileStatus Status;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerOwner;

	//Due Float per posione (x,y)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileGridPosition;


	//il tick è false
//public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

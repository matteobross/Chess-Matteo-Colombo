// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

UCLASS()
class CHESS_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AGameField();
	// 
// Sets default values for this actor's properties
	AGameField();

	//Dichiarazioni variabili, da include il file di unreal Tile.h
	//Creo un'array con le tile
	UPROPERTY(Transient)
	TArray<ATile*> TileArray;

	//Tmap struttura dati che usa x,y come chiave
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	//Spazio tra le tile relativo alle tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// Dimensione scacchiera
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Condizione di vittoria
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WinSize;

	// Per la safety di TileClass
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;

	// spazio tra le tile
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellPadding;

	//dimensione tile
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize;
	///////////////////////////////////////////////////////////
	// 
	// Crea la normalizzazione
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Metto notassigned nello Tile status
	UFUNCTION(BlueprintCallable)
	void ResetField();

	// GEneratore scacchiera
	void GenerateField();

	// Mi ridà (x,y) con il click sinistro
	FVector2D GetPosition(const FHitResult& Hit);

	//Array con il puntatore delle tile nell'array
	TArray<ATile*>& GetTileArray();

	// Posizione centrata sulla scacchiera data X e Y
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// X,Y data posizione centrata sulla schacchiera
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;
/// 
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Controllo mossa vincente
	bool IsWinPosition(const FVector2D Position) const;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!check if is a win line
	inline bool IsWinLine(const FVector2D Begin, const FVector2D End) const;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!checking if is a valid field position
	inline bool IsValidPosition(const FVector2D Position) const;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! get a line given a begin and end positions
TArray<int32> GetLine(const FVector2D Begin, const FVector2D End) const;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! check if a line contains all equal elements
	bool AllEqual(const TArray<int32>& Array) const;

	//Creazioen scacchiera visibile
	 
	// Dimensione scacchiera
	int32 BoardSize;

	// Tile della scacchiera
	float CellSize;

	// Mateiaale celle bianche
	UPROPERTY(EditAnywhere)
	UMaterial* WhiteMaterial;

	// Materiale celle nere
	UPROPERTY(EditAnywhere)
	UMaterial* BlackMaterial;

	void CreateChessBoard();

	//Per far spawnare i pedoni (già in posizione)
	void SpawnPawn(int32 Row, int32 Column);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

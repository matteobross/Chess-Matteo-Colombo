// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"
#include <GameMapsSettings.h>
#include <AI/NavigationSystemBase.h>
#include "ChessPawn.h"
#include "Chess_gamemode.h"


// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Per la scacchiera visibile
	BoardSize = 8;
	CellSize = 120;

	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!size of winnning line
	WinSize = 3;
	// scacchiera 8x8                       DA METTERE PARI E DIPARI PER SCACCHIRA
	Size = BoardSize;
	// dimensione tile
	TileSize = CellSize;
	// Spazio tra letile
	CellPadding = 20;
}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalizzazione spazio tra le celle
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
	CreateChessBoard();

//Per far spawnare i pedoni solo nei punti iniziali
	for (int32 Row = 0; Row < BoardSize; ++Row)
	{
		for (int32 Column = 0; Column < BoardSize; ++Column)
		{
			SpawnPawn(Row, Column);
		}
	}
}

void AGameField::ResetField()
{
	for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, ETileStatus::EMPTY);
	}
	//qui ci sarebbe da mettere il messaggio broadcast per resettare
	
	AChess_gamemode* GameMode = Cast<AChess_gamemode>(GetWorld()->GetAuthGameMode()); 
	GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
}

//Tile è la griglia dove venono salvate le posizioni.
void AGameField::GenerateField() 
{
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}

//Creazione della scacchiera visibile è dopo


FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	return FVector2D(x,y);
}

bool AGameField::IsWinPosition(const FVector2D Position) const
{
	//condizione vittoria degli scacchi
	return false;
}

//POi da aggiungere ci sono tutte le condizioni di vittoria


void AGameField::CreateChessBoard()
{

	{
		// Loop through rows and columns to create the chessboard
		for (int32 Row = 0; Row < BoardSize; ++Row)
		{
			for (int32 Col = 0; Col < BoardSize; ++Col)
			{
				// Calculate the location of the cell
				FVector CellLocation = FVector(Col * CellSize, Row * CellSize, 0.0f);

				// Create a static mesh component for the cell
				UStaticMeshComponent* CellMesh = NewObject<UStaticMeshComponent>(this);
				CellMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
				CellMesh->SetWorldLocation(CellLocation);
				CellMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

				// Apply alternating materials based on row and column
				UMaterial* CellMaterial = (Row + Col) % 2 == 0 ? WhiteMaterial : BlackMaterial;
				CellMesh->SetMaterial(0, CellMaterial);
			}
		}
	}
}

//Per far spawnare i pedoni
void AGameField::SpawnPawn(int32 Row, int32 Column)
{
	// Calculate the location for the pawn based on the row and column
	FVector SpawnLocation = FVector(TileSize * Column, TileSize * Row, 0.0f);

	// Spawn the pawn
	AChessPawn* NewPawn = GetWorld()->SpawnActor<AChessPawn>(AChessPawn::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	if (NewPawn)
}

// Called every frame
void AGameField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


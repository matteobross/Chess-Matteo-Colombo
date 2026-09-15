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

	// dimensione della linea vincente (quanti pezzi allineati servono per vincere)
	WinSize = 3;
	// scacchiera 8x8
	Size = BoardSize;
	// dimensione tile
	TileSize = CellSize;
	// Spazio tra le tile
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

	AChess_gamemode* GameMode = GetWorld()->GetGameState<AChess_gamemode>();
	if (GameMode)
	{
		GameMode->IsGameOver = false;
		GameMode->MoveCounter = 0;
		GameMode->ChoosePlayerAndStartGame();
	}
}

//Tile e' la griglia dove vengono salvate le posizioni.
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

bool AGameField::IsValidPosition(const FVector2D Position) const
{
	const int32 X = FMath::RoundToInt(Position.X);
	const int32 Y = FMath::RoundToInt(Position.Y);
	return X >= 0 && X < Size && Y >= 0 && Y < Size;
}

TArray<int32> AGameField::GetLine(const FVector2D Begin, const FVector2D End) const
{
	// Legge la sequenza di "owner" delle tile lungo la linea retta da Begin a End
	// (Begin/End devono essere sulla stessa riga, colonna o diagonale).
	TArray<int32> Line;

	const int32 BeginX = FMath::RoundToInt(Begin.X);
	const int32 BeginY = FMath::RoundToInt(Begin.Y);
	const int32 EndX = FMath::RoundToInt(End.X);
	const int32 EndY = FMath::RoundToInt(End.Y);

	const int32 StepX = FMath::Sign(EndX - BeginX);
	const int32 StepY = FMath::Sign(EndY - BeginY);
	const int32 Steps = FMath::Max(FMath::Abs(EndX - BeginX), FMath::Abs(EndY - BeginY));

	int32 CurrentX = BeginX;
	int32 CurrentY = BeginY;

	for (int32 i = 0; i <= Steps; i++)
	{
		const FVector2D Current(CurrentX, CurrentY);

		if (!IsValidPosition(Current))
		{
			return TArray<int32>();
		}

		if (ATile* const* FoundTile = TileMap.Find(Current))
		{
			Line.Add((*FoundTile)->GetOwner());
		}
		else
		{
			return TArray<int32>();
		}

		CurrentX += StepX;
		CurrentY += StepY;
	}

	return Line;
}

bool AGameField::AllEqual(const TArray<int32>& Array) const
{
	if (Array.Num() == 0)
	{
		return false;
	}

	const int32 FirstOwner = Array[0];
	// NOT_ASSIGNED significa "tile vuota": una linea di caselle vuote non e' una linea vincente
	if (FirstOwner == NOT_ASSIGNED)
	{
		return false;
	}

	for (const int32 Owner : Array)
	{
		if (Owner != FirstOwner)
		{
			return false;
		}
	}

	return true;
}

bool AGameField::IsWinLine(const FVector2D Begin, const FVector2D End) const
{
	if (!IsValidPosition(Begin) || !IsValidPosition(End))
	{
		return false;
	}

	return AllEqual(GetLine(Begin, End));
}

bool AGameField::IsWinPosition(const FVector2D Position) const
{
	// Controlla se l'ultima pedina piazzata in Position fa parte di una linea
	// di WinSize pedine dello stesso giocatore, in una delle 4 direzioni possibili:
	// orizzontale, verticale, diagonale principale, diagonale secondaria.
	const FVector2D RoundedPosition(FMath::RoundToInt(Position.X), FMath::RoundToInt(Position.Y));

	if (!IsValidPosition(RoundedPosition))
	{
		return false;
	}

	const TArray<FVector2D> Directions = {
		FVector2D(1, 0),
		FVector2D(0, 1),
		FVector2D(1, 1),
		FVector2D(1, -1)
	};

	for (const FVector2D& Dir : Directions)
	{
		// Fa scorrere una finestra di WinSize caselle lungo la direzione, passante per Position
		for (int32 Offset = -(WinSize - 1); Offset <= 0; Offset++)
		{
			const FVector2D Begin = RoundedPosition + Dir * Offset;
			const FVector2D End = RoundedPosition + Dir * (Offset + WinSize - 1);

			if (IsWinLine(Begin, End))
			{
				return true;
			}
		}
	}

	return false;
}

void AGameField::CreateChessBoard()
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

//Per far spawnare i pedoni
void AGameField::SpawnPawn(int32 Row, int32 Column)
{
	// Calculate the location for the pawn based on the row and column
	FVector SpawnLocation = FVector(TileSize * Column, TileSize * Row, 0.0f);

	// Spawn the pawn
	AChessPawn* NewPawn = GetWorld()->SpawnActor<AChessPawn>(AChessPawn::StaticClass(), SpawnLocation, FRotator::ZeroRotator);
	if (NewPawn)
	{
		// TODO: eventuale inizializzazione del pedone appena spawnato (tipo di pezzo, proprietario, ecc.)
	}
}

// Called every frame
void AGameField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

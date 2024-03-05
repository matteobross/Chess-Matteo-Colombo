// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_gamemode.h"
#include "Chess_PlayerController.h"
#include "EngineUtils.h"

AChess_gamemode::AChess_gamemode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass();
	DefaultPawnClass = AHumanPlayer::StaticClass();
	FieldSize = 8;
}

void AChess_gamemode::BeginPlay()
{
	IsGameOver = false;

	MoveCounter = 0;

	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));
	
	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());


	// Da ricordarsi che il giocatore umano = 0
	Players.Add(HumanPlayer);
	


	// AI player = 1
	Players.Add(AI);

	this->ChoosePlayerAndStartGame();
}

void AChess_gamemode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = FMath::RandRange(0, Players.Num() - 1);

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		Players[i]->Sign = i == CurrentPlayer ? ESign::X : ESign::O;
	}
	MoveCounter += 1;
	Players[CurrentPlayer]->OnTurn();
}

void AChess_gamemode::SetCellSign(const int32 PlayerNumber, const FVector& SpawnPosition)
{
	if (IsGameOver || PlayerNumber != CurrentPlayer)
	{
		return;

		UClass* SignActor = Players[CurrentPlayer]->Sign == ESign::X ? SignXActor : SignOActor;
		FVector Location = GField->GetActorLocation() + SpawnPosition + FVector(0, 0, 10);
		GetWorld()->SpawnActor(SignActor, &Location);
		//ancora da inserire condizione vittoria degli scachi
		if (GField->IsWinPosition(GField->GetXYPositionByRelativeLocation(SpawnPosition)))
		{
			IsGameOver = true;
			Players[CurrentPlayer]->OnWin();
			for (int32 i = 0; i < Players.Num(); i++)
			{
				if (i != CurrentPlayer)
				{
					Players[i]->OnLose();
				}
			}
		}
		else if (MoveCounter == (FieldSize * FieldSize))
		{
			// add a timer (3 seconds)
			FTimerHandle TimerHandle;

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					// function to delay
					GField->ResetField();
				}, 3, false);
		}
		else
		{
			TurnNextPlayer();
		}


}
	;;

	//NON SO IL PERCHè SE CANCELLO QUESTA NON FUNZIONA QUELLA SOOT, SARà UNA COSADELLE PARENTESI
int32 AChess_gamemode::GetNextPlayer(int32 Player)
{
Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}



void AChess_gamemode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}


int32 AChess_gamemode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomPlayer.h"
#include <Tile.h>

// Sets default values
ARandomPlayer::ARandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!GameInstance = Cast<UTTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void ARandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARandomPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			TArray<ATile*> FreeCells;
		//  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	ATTT_GameMode* GameMode = (ATTT_GameMode*)(GetWorld()->GetAuthGameMode());
			//for (auto& CurrTile : GameMode->GField->GetTileArray())
			{
				//if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
				{
					FreeCells.Add(CurrTile);
				}
			}

			if (FreeCells.Num() > 0)
			{
				int32 RandIdx = FMath::Rand() % FreeCells.Num();
				FVector Location = GameMode->GField->GetRelativeLocationByXYPosition((FreeCells[RandIdx])->GetGridPosition()[0], (FreeCells[RandIdx])->GetGridPosition()[1]);
				FreeCells[RandIdx]->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);

				GameMode->SetCellSign(PlayerNumber, Location);

			}
		}, 3, false);
}

void ARandomPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void ARandomPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	 GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}


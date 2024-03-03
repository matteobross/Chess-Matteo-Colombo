// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Chess_gamemode.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChess_gamemode : public AGameStateBase
{
	GENERATED_BODY()
public:
	// Partita Finita
	bool IsGameOver;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Manca la I Player Interface
	TArray<ITTT_PlayerInterface*> Players;

	//Dichiaro le variabili per il giocatore e per il numero di mosse
	int32 CurrentPlayer;
	int32 MoveCounter;

	//Mi serve per fare riferimento alla Gamefield, TSubclassOf per la safery
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;

	// !!!!!!!!!!!!!!!!!!Grandezza della scacchiera
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	// Riferimento alla GAmefield
	UPROPERTY(VisibleAnywhere)
	AGameField* GField;

	//
	//DA FARE CLASSE A. ACTOR CON I PEZZI DELLA SCACCHIERA 
	//QUI SOTTO SONO RICHIAMATI I PEZZI X E O DEL TRIS
	// 
	// 
	//Tiro fuori altre due Subclass da Actor, TSubclassOf per safety
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SignXActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SignOActor;

	AChess_gamemode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Sceglie un giocatore alla partenza
	void ChoosePlayerAndStartGame();

	// set the cell sign and the position 
	void SetCellSign(const int32 PlayerNumber, const FVector& SpawnPosition);

	// Prendi il giocatore nuovo
	int32 GetNextPlayer(int32 Player);

	// Fine turno
	void TurnNextPlayer();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chess_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UChess_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//Dichiarazioni variabili 
	// Punti HumanPlayer
	UPROPERTY(EditAnywhere)
	int32 ScoreHumanPlayer = 0;

	// Punti AIPlayer
	UPROPERTY(EditAnywhere)
	int32 ScoreAiPlayer = 0;

	// messaggio turno di:
	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player";

	// Incrementa Humanplayer
	void IncrementScoreHumanPlayer();

	// Incremento AI player
	void IncrementScoreAiPlayer();

	// GET Punteggio humanplayer
	UFUNCTION(BlueprintCallable)
	int32 GetScoreHumanPlayer();

	// GET Punteggio AiPlayer
	UFUNCTION(BlueprintCallable)
	int32 GetScoreAiPlayer();

	// GET turn message
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage();

	// SET turn message
	UFUNCTION(BlueprintCallable)
	void SetTurnMessage(FString Message);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPawn.generated.h"

UCLASS()
class CHESS_API AChessPawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//DA mettere funzione binding con controller

	// Trovato su internet per la mesh da vedere se funziona
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ChessPawnMesh;

	//da aggiungere funzione che aggiorna la posizione

};

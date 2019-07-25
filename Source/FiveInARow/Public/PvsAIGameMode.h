// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Chunk.h"
#include "PvsAIGameMode.generated.h"

struct PriorityArray
{
private:
	int ArrData[8][8];
public:
	PriorityArray()
	{
		for (int nCol = 0; nCol < 8; nCol++)
			for (int nRow = 0; nRow < 8; nRow++)
				ArrData[nRow][nCol] = 0;
	}
	int operator()(const int nCol, const int nRow) { return ArrData[nRow][nCol]; };
	void operator()(const int nCol, const int nRow, int value) { ArrData[nRow][nCol] = value; };
	void operator()()
	{
		for (int nCol = 0; nCol < 8; nCol++)
			for (int nRow = 0; nRow < 8; nRow++)
				ArrData[nRow][nCol] = 0;
	}
};


UENUM(BlueprintType)
enum class RoundResult : uint8
{
	RR_Draw = 0 	UMETA(DisplayName = "Draw"),
	RR_WinPlayer 	UMETA(DisplayName = "WinPlayer"),
	RR_WinAI	UMETA(DisplayName = "WinAI")
};


USTRUCT(BlueprintType)
struct FChunkRows
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2D Array")
		TArray<AChunk*> Cols;

	AChunk* operator[] (int32 i)
	{
		return Cols[i];
	}

	void Add(AChunk* chunk) {
		Cols.Add(chunk);
	}

	TArray<AChunk*> GetColArray() const
	{
		return Cols;
	}

	FChunkRows()
	{}
};


/**
 * 
 */
UCLASS()
class FIVEINAROW_API APvsAIGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	PriorityArray PriorArr;

public:

	APvsAIGameMode();


protected:
	
	UPROPERTY(BlueprintReadOnly)
	int32 MaxBoardSize;
	
	UPROPERTY(BlueprintReadOnly)
	int32 WinLineCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode")
	int32 Difficult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	int32 FillChunksCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	TArray<FChunkRows> ChunkOfRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
	RoundResult ResultOfRound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode")
	TArray<int32> MemoryOfMoves;

	UFUNCTION(BlueprintCallable)
	TArray<AChunk*>GetColArray(int32 i) const;

	UFUNCTION(BlueprintCallable)
	void AddToCol(int32 i, AChunk* chunk);

	UFUNCTION(BlueprintCallable)
	void CreateBoard();

	UFUNCTION(BlueprintCallable)
	void AddToMoveMemory(int x, int y, bool isPlayer);

	UFUNCTION(BlueprintCallable)
	bool ReturnMove(int ReturnCount);

	///Check Winner

	UFUNCTION(BlueprintCallable)
	bool isWin(int32 x, int32 y, int neededcount);

	bool isRowCountOfChunk(int32 x, int32 y, int neededcount);

	bool isColumnCountOfChunk(int32 x, int32 y, int neededcount);

	bool isMainDiagnolOfChunk(int32 x, int32 y, int neededcount);

	bool isReverseDiagnolOfChunk(int32 x, int32 y, int neededcount);

	///AI

	UFUNCTION(BlueprintCallable)
	AChunk* PCMove();

	void CalculatePriority();	
	
	bool CompareChunks(AChunk* A, AChunk* B);

};

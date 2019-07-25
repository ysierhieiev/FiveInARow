#include "PvsAIGameMode.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

APvsAIGameMode::APvsAIGameMode()
{
	MaxBoardSize = 8;
	WinLineCount = 5;
	FillChunksCount = 0;
	MemoryOfMoves.Init(-1, 12);
}

TArray<AChunk*> APvsAIGameMode::GetColArray(int32 i) const
{
	return ChunkOfRows[i].GetColArray();
}

void APvsAIGameMode::AddToCol(int32 i, AChunk* chunk)
{
	ChunkOfRows[i].Add(chunk);
}

void APvsAIGameMode::CreateBoard()
{
	FVector Location(0.0f, 0.0f, 10.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0; i < 5; ++i)
	{
		ChunkOfRows.Add(FChunkRows());
		for (int j = 0; j < 5; ++j)
		{
			ChunkOfRows[i].Add(GetWorld()->SpawnActor<AChunk>(Location, Rotation, SpawnInfo));
			
			Location += FVector(50.f, 0.f, 0.f);
		}
		Location += FVector(0.f, 50.f, 0.f);
	}
}

void APvsAIGameMode::AddToMoveMemory(int x, int y, bool isPlayer)
{
	if (isPlayer)
	{		
		for (int i = 0; i < 6; ++i)
		{
			MemoryOfMoves.Swap(i, (i + 2));
		}
		MemoryOfMoves[4] = x;
		MemoryOfMoves[5] = y;
	}
	else
	{
		for (int i = 6; i < 10; ++i)
		{
			MemoryOfMoves.Swap(i, (i + 2));
		}
		MemoryOfMoves[10] = x;
		MemoryOfMoves[11] = y;
	}
}

bool APvsAIGameMode::ReturnMove(int ReturnCount)
{
	if (ReturnCount == 0 || MemoryOfMoves[5]==-1) return false;

	ChunkOfRows[MemoryOfMoves[4]][MemoryOfMoves[5]]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
	ChunkOfRows[MemoryOfMoves[4]][MemoryOfMoves[5]]->SetEmptyMesh();
	for (int i = 5; i > 1; --i)
	{
		MemoryOfMoves.Swap(i, (i - 2));
	}
	ChunkOfRows[MemoryOfMoves[10]][MemoryOfMoves[11]]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
	ChunkOfRows[MemoryOfMoves[10]][MemoryOfMoves[11]]->SetEmptyMesh();
	for (int i = 11; i > 7; --i)
	{
		MemoryOfMoves.Swap(i, (i - 2));
	}
	return true;
}

bool APvsAIGameMode::isWin(int32 x, int32 y, int neededcount)
{
	return (isRowCountOfChunk(x , y, neededcount) || isColumnCountOfChunk(x, y, neededcount) || isMainDiagnolOfChunk(x, y, neededcount) || isReverseDiagnolOfChunk(x,y, neededcount));
}

bool APvsAIGameMode::isRowCountOfChunk(int32 x, int32 y, int neededcount)
{
	int temp = 1;
	int i = 1;
	while ((x - i) >= 0 && CompareChunks(ChunkOfRows[x - i][y], ChunkOfRows[x][y])) 
	{
		temp++;
		i++;		
	}
	i = 1;
	while ((x + i) < MaxBoardSize && CompareChunks(ChunkOfRows[x + i][y], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	return (temp >= neededcount);
}

bool APvsAIGameMode::isColumnCountOfChunk(int32 x, int32 y, int neededcount)
{
	int temp = 1;
	int i = 1;
	while ((y - i) >= 0 && CompareChunks(ChunkOfRows[x][y - i], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	i = 1;
	while ((y + i) < MaxBoardSize && CompareChunks(ChunkOfRows[x][y + i], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	return (temp >= neededcount);
}

bool APvsAIGameMode::isMainDiagnolOfChunk(int32 x, int32 y, int neededcount)
{
	int temp = 1;
	int i = 1;
	while (x - i >= 0 && y - i >= 0 && CompareChunks(ChunkOfRows[x - i][y - i], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	i = 1;
	while (x + i < MaxBoardSize && y + i < MaxBoardSize && CompareChunks(ChunkOfRows[x + i][y + i], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	return (temp >= neededcount);
}

bool APvsAIGameMode::isReverseDiagnolOfChunk(int32 x, int32 y, int neededcount)
{
	int temp = 1;
	int i = 1;
	while (x - i >= 0 && y + i < MaxBoardSize && CompareChunks(ChunkOfRows[x - i][y + i], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	i = 1;
	while (x + i < MaxBoardSize && y - i >= 0 && CompareChunks(ChunkOfRows[x + i][y - i], ChunkOfRows[x][y]))
	{
		temp++;
		i++;
	}
	return (temp >= neededcount);
}

AChunk* APvsAIGameMode::PCMove()
{
	int chance = 0, maxy = 0, maxx = 0, maxo = -1;
	switch (Difficult)
	{
	case 0: chance = 25; break;
	case 1: chance = 65; break;
	case 2: chance = 100; break;
	default: chance = 10;
	}

	if (FMath::RandRange(1, 100 / chance) == 1 ? true : false)
	{
		PriorArr();
		CalculatePriority();
		for (int cv = 0; cv < MaxBoardSize; cv++)
		{
			for (int cv2 = 0; cv2 < MaxBoardSize; cv2++)
			{
				if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
				{
					if (maxo < PriorArr(cv, cv2))
					{
						maxo = PriorArr(cv, cv2);
						maxy = cv;
						maxx = cv2;
					}
				}
			}
		}
	}
	else
	{
		do
		{
			maxy = FMath::RandRange(0, 7);
			maxx = FMath::RandRange(0, 7);
		}
		while(ChunkOfRows[maxy][maxx]->GetCurrentChunkStatus() != EChunkStatus::CS_Empty);
	}	

	ChunkOfRows[maxy][maxx]->SetCurrentChunkStatus(EChunkStatus::CS_AI);
	FillChunksCount++;

	return ChunkOfRows[maxy][maxx];
}

void APvsAIGameMode::CalculatePriority()
{
	for (auto cv = 0; cv < MaxBoardSize; cv++)
		for (auto cv2 = 0; cv2 < MaxBoardSize; cv2++)
		{
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_AI);
				if (isWin(cv, cv2, 4))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 20);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			else
				PriorArr(cv, cv2, -1);
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Player);
				if (isWin(cv, cv2, 4))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 9);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_AI);
				if (isWin(cv, cv2, 3))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 1);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			else
				PriorArr(cv, cv2, -1);
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Player);
				if (isWin(cv, cv2, 3))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 2);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_AI);
				if (isWin(cv, cv2, 5))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 21);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			else
				PriorArr(cv, cv2, -1);
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Player);
				if (isWin(cv, cv2, 5))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 20);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_AI);
				if (isWin(cv, cv2, 2))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 1);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
			else
				PriorArr(cv, cv2, -1);
			if (ChunkOfRows[cv][cv2]->GetCurrentChunkStatus() == EChunkStatus::CS_Empty)
			{
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Player);
				if (isWin(cv, cv2, 2))
				{
					PriorArr(cv, cv2, PriorArr(cv, cv2) + 1);
				}
				ChunkOfRows[cv][cv2]->SetCurrentChunkStatus(EChunkStatus::CS_Empty);
			}
		}
}

bool APvsAIGameMode::CompareChunks(AChunk* A, AChunk* B)
{
	if ((A == nullptr) || (B == nullptr)) { return false; }
	return A->GetCurrentChunkStatus() == B->GetCurrentChunkStatus();
}

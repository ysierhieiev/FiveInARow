// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

UENUM(BlueprintType)		
enum class EChunkStatus : uint8
{
	CS_Empty = 0 	UMETA(DisplayName = "Empty"),
	CS_Player 	UMETA(DisplayName = "Player"),
	CS_AI	UMETA(DisplayName = "AI"),
	CS_Player_Projection 	UMETA(DisplayName = "Player_Projection")
};

UCLASS()
class FIVEINAROW_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	UFUNCTION(BlueprintCallable)
	EChunkStatus GetCurrentChunkStatus() const;

	void SetCurrentChunkStatus(EChunkStatus NewStatus);

	UFUNCTION(BlueprintCallable)
	void SetPositionInBoard(int X, int Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	   
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Chunk, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Chunk, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StatusDemonstration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chunk)
	EChunkStatus CurrentChunkStatus;

	UPROPERTY(EditDefaultsOnly, Category = Chunk)
	TArray<UStaticMesh*> StatusMeshs;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chunk)
	int Xposition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chunk)
	int Yposition;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetEmptyMesh();

	FORCEINLINE bool operator==(const AChunk& Other) const
	{
		return CurrentChunkStatus == Other.CurrentChunkStatus;
	}
};

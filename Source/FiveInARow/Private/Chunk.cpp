// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AChunk::AChunk()
{


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	StatusDemonstration = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatusDemonstration"));
	StatusDemonstration->SetRelativeLocation(FVector(0.f, 0.f, 105.f));
	StatusDemonstration->SetupAttachment(RootComponent);	

}

EChunkStatus AChunk::GetCurrentChunkStatus() const
{
	return CurrentChunkStatus;
}

void AChunk::SetCurrentChunkStatus(EChunkStatus NewStatus)
{
	CurrentChunkStatus = NewStatus;
}


void AChunk::SetPositionInBoard(int X, int Y)
{
	Xposition = X;
	Yposition = Y;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
}




// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
#include "Misc/DateTime.h"   // FDateTime

AMyActor::AMyActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMyActor::BeginPlay()
{
    Super::BeginPlay();

	DefaultScale = GetActorScale3D();
	X = GetActorLocation().X;
	Y = GetActorLocation().Y;

    UE_LOG(LogTemp, Log, TEXT("[RandomWalker] Spawned at (0, 0)"));

    if (Seed == 0)
    {
        const int32 TimeSeed = static_cast<int32>(FDateTime::Now().GetTicks() & 0xFFFFFFFF);
        Rnd.Initialize(TimeSeed);
        UE_LOG(LogTemp, Log, TEXT("[RandomWalker] Seed(auto): %d"), TimeSeed);
    }
    else
    {
        Rnd.Initialize(Seed);
        UE_LOG(LogTemp, Log, TEXT("[RandomWalker] Seed(manual): %d"), Seed);
    }

    MovesDone = 0; TimeAcc = 0.f;
}

void AMyActor::Tick(float DeltaTime) // 완
{
    Super::Tick(DeltaTime);

    // 스케일 복원 처리
    if (bScaleReturning) {
		ScaleElapsed += DeltaTime;
        const float Alpha = FMath::Clamp(ScaleElapsed / ScaleReturnDuration, 0.f, 1.f);
        const FVector PeakScale = DefaultScale * ScalePeakMultiplier;
        const FVector NewScale = FMath::Lerp(PeakScale, DefaultScale, Alpha);
		SetActorScale3D(NewScale);

        if (Alpha >= 1.f) {
            bScaleReturning = false;
            SetActorScale3D(DefaultScale);
        }
    }

    if (MovesDone >= NumMoves)
    {
        // 다 끝났으면 더 이상 틱 돌리지 않기(선택)
        SetActorTickEnabled(false);
        return;
    }

    TimeAcc += DeltaTime;
    while (TimeAcc >= MoveInterval && MovesDone < NumMoves)
    {
        TimeAcc -= MoveInterval;
        Move();
    }
}

int32 AMyActor::Step() // 완
{
    return Rnd.RandRange(-1, 1);
}

void AMyActor::Move()
{
    StepX = Step();
    StepY = Step();

    UE_LOG(LogTemp, Log, TEXT("Previous X: %f, Previous Y: %f"), X, Y);
    UE_LOG(LogTemp, Log, TEXT("Moving %f in the X direction, %f in the Y direction."), StepX * GridUnit, StepY * GridUnit);

    //X += StepX;
    //Y += StepY;

    UE_LOG(LogTemp, Log, TEXT("Current position: (%f, %f)"), X + StepX * GridUnit, Y + StepY * GridUnit);
    const FVector Delta(StepX * GridUnit, StepY * GridUnit, 0.f);

    const FVector PrevLoc = GetActorLocation();
    const bool bMoved = SetActorLocation(PrevLoc + Delta, /*bSweep=*/false, /*OutHit=*/nullptr, ETeleportType::None);

    ++MovesDone;

    // 10회 이동후 각 스텝마다 이벤트 발생
    if (MovesDone >= 10) {
        UE_LOG(LogTemp, Log, TEXT("%d Counts Moves. (%f, %f) -> (%f, %f). MoveDistance : %f"), MovesDone, X, Y, X + StepX * GridUnit, Y + StepY * GridUnit, distance(X, Y, X + StepX * GridUnit, Y + StepY* GridUnit));
        if (Rnd.RandRange(0, 1) == 1) {
            event();
        }
    }

    X = X + StepX * GridUnit;
    Y = Y + StepY * GridUnit;

    // 10회 이동마다 이벤트 출력
    if (MovesDone && MovesDone % 10 == 0) {    
        UE_LOG(LogTemp, Log, TEXT("Excution EventCounts : %d"), eventCount);
    }
}

void AMyActor::event() { // 이벤트 생성 필요
    eventCount++;
	SetActorScale3D(DefaultScale * ScalePeakMultiplier);

	bScaleReturning = true;
	ScaleElapsed = 0.f;

    UE_LOG(LogTemp, Log, TEXT("Event occurred. actor scale multiplied by %f"), ScalePeakMultiplier);
}

float AMyActor::distance(float x, float y, float nx, float ny) { // 완
	return sqrt((nx - x) * (nx - x) + (ny - y) * (ny - y));
}

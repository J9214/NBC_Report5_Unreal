#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"   // FRandomStream 멤버 때문에 헤더에서 포함
#include "MyActor.generated.h"

UCLASS()
class PLAYGROUND_API AMyActor : public AActor
{
    GENERATED_BODY()
public:
    AMyActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, Category="Move")
    int32 NumMoves = 10;                 // 기본 10회

    UPROPERTY(EditAnywhere, Category="Move")
    int32 Seed = 0;

    UPROPERTY(EditAnywhere, Category = "Move")
    int32 eventTerm = 10;

    UPROPERTY(EditAnywhere, Category="Move")
    float MoveInterval = 0.5f;           // 0.5초마다 이동

    UPROPERTY(EditAnywhere, Category = "Move", meta = (ClampMin = "1", UIMin = "1"))
    float GridUnit = 100.f; // 한 칸의 실제 거리(cm)

    UPROPERTY(EditAnywhere, Category = "FX")
    float  ScaleReturnDuration = 0.5f; // 0.5초 동안 원래로

    UPROPERTY(EditAnywhere, Category = "FX")
    float  ScalePeakMultiplier = 2.0f; // 순간 2배

private:
    float X = 0, Y = 0;
    int32 StepX = 0, StepY = 0;
    int32 MovesDone = 0;
    int32 eventCount = 0;
    float TimeAcc = 0.f;

    bool   bScaleReturning = false;
    float  ScaleElapsed = 0.f;
    FVector DefaultScale = FVector::OneVector;

    int32 Step();        // 0 또는 1 반환
    void Move();     // 한 스텝만 수행
    float distance(float x, float y, float nx, float ny);
    void event();

    FRandomStream Rnd;
};

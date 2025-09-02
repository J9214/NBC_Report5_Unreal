#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"   // FRandomStream ��� ������ ������� ����
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
    int32 NumMoves = 10;                 // �⺻ 10ȸ

    UPROPERTY(EditAnywhere, Category="Move")
    int32 Seed = 0;

    UPROPERTY(EditAnywhere, Category = "Move")
    int32 eventTerm = 10;

    UPROPERTY(EditAnywhere, Category="Move")
    float MoveInterval = 0.5f;           // 0.5�ʸ��� �̵�

    UPROPERTY(EditAnywhere, Category = "Move", meta = (ClampMin = "1", UIMin = "1"))
    float GridUnit = 100.f; // �� ĭ�� ���� �Ÿ�(cm)

    UPROPERTY(EditAnywhere, Category = "FX")
    float  ScaleReturnDuration = 0.5f; // 0.5�� ���� ������

    UPROPERTY(EditAnywhere, Category = "FX")
    float  ScalePeakMultiplier = 2.0f; // ���� 2��

private:
    float X = 0, Y = 0;
    int32 StepX = 0, StepY = 0;
    int32 MovesDone = 0;
    int32 eventCount = 0;
    float TimeAcc = 0.f;

    bool   bScaleReturning = false;
    float  ScaleElapsed = 0.f;
    FVector DefaultScale = FVector::OneVector;

    int32 Step();        // 0 �Ǵ� 1 ��ȯ
    void Move();     // �� ���ܸ� ����
    float distance(float x, float y, float nx, float ny);
    void event();

    FRandomStream Rnd;
};

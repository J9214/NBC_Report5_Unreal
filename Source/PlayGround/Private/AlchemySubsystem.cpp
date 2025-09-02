// AlchemySubsystem.cpp
#include "AlchemySubsystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Containers/UnrealString.h"

void UAlchemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("AlchemySubsystem Initialized"));
}

void UAlchemySubsystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Log, TEXT("AlchemySubsystem Deinitialized"));
}

void UAlchemySubsystem::AddRecipe(FName PotionName, const TArray<FString>& Ingredients)
{
    FPotionRecipe NewRecipe;
    NewRecipe.PotionName = PotionName;
    NewRecipe.Ingredients = Ingredients;
    Recipes.Add(NewRecipe);

    UE_LOG(LogTemp, Log, TEXT(">> ���ο� ������ '%s'��(��) �߰��Ǿ����ϴ�."), *PotionName.ToString());

    InitializeStock(PotionName); // �߰� �� ��� �ڵ� �ʱ�ȭ(3��)
}

bool UAlchemySubsystem::GetRecipeByName(FName PotionName, FPotionRecipe& OutRecipe) const
{
    const FPotionRecipe* Found = Recipes.FindByPredicate(
        [PotionName](const FPotionRecipe& R) { return R.PotionName == PotionName; });
    if (Found)
    {
        OutRecipe = *Found;
        return true;
    }
    return false;
}

void UAlchemySubsystem::GetRecipesByIngredient(const FString& Ingredient, TArray<FPotionRecipe>& OutRecipes) const
{
    OutRecipes.Reset();
    for (const FPotionRecipe& R : Recipes)
    {
        if (R.Ingredients.Contains(Ingredient))
        {
            OutRecipes.Add(R);
        }
    }
}

void UAlchemySubsystem::InitializeStock(FName PotionName)
{
    PotionStock.FindOrAdd(PotionName) = MAX_STOCK;
}

bool UAlchemySubsystem::HasPotion(FName PotionName) const
{
    return PotionStock.Contains(PotionName);
}

int32 UAlchemySubsystem::GetPotionCount(FName PotionName) const
{
    if (const int32* Cnt = PotionStock.Find(PotionName))
    {
        return *Cnt;
    }
    return -1; // �����
}

bool UAlchemySubsystem::DispensePotion(FName PotionName)
{
    if (int32* Cnt = PotionStock.Find(PotionName))
    {
        if (*Cnt > 0)
        {
            --(*Cnt);
            UE_LOG(LogTemp, Log, TEXT("%s��(��) 1�� �����Ͽ����ϴ�. ���� ����: %d"), *PotionName.ToString(), *Cnt);
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s ������ ��� �����ϴ�."), *PotionName.ToString());
            return false;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("%s ��(��) ������� �ʽ��ϴ�."), *PotionName.ToString());
    return false;
}

void UAlchemySubsystem::ReturnPotion(FName PotionName)
{
    if (int32* Cnt = PotionStock.Find(PotionName))
    {
        if (*Cnt >= MAX_STOCK)
        {
            UE_LOG(LogTemp, Warning, TEXT("������ ������ �����մϴ�."));
        }
        else
        {
            (*Cnt)++;
            UE_LOG(LogTemp, Log, TEXT("%s�� ������ 1�� ��ȯ�Ǿ����ϴ�. ���� ����: %d"), *PotionName.ToString(), *Cnt);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s ��(��) ������� �ʽ��ϴ�."), *PotionName.ToString());
    }
}

void UAlchemySubsystem::GetDispensableByIngredient(const FString& Ingredient, TMap<FName, int32>& OutList) const
{
    OutList.Reset();

    TArray<FPotionRecipe> Candidates;
    GetRecipesByIngredient(Ingredient, Candidates);

    for (const FPotionRecipe& R : Candidates)
    {
        if (const int32* Cnt = PotionStock.Find(R.PotionName))
        {
            if (*Cnt > 0)
            {
                OutList.Add(R.PotionName, *Cnt);
            }
        }
    }
}

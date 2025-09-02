// AlchemySubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AlchemySubsystem.generated.h"

USTRUCT(BlueprintType)
struct FPotionRecipe
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName PotionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Ingredients;
};

UCLASS(BlueprintType)
class PLAYGROUND_API UAlchemySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Subsystem 수명주기
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // 레시피
    UFUNCTION(BlueprintCallable, Category="Alchemy|Recipe")
    void AddRecipe(FName PotionName, const TArray<FString>& Ingredients);

    UFUNCTION(BlueprintCallable, Category="Alchemy|Recipe")
    bool GetRecipeByName(FName PotionName, FPotionRecipe& OutRecipe) const;

    UFUNCTION(BlueprintCallable, Category="Alchemy|Recipe")
    void GetRecipesByIngredient(const FString& Ingredient, TArray<FPotionRecipe>& OutRecipes) const;

    UFUNCTION(BlueprintCallable, Category="Alchemy|Recipe")
    const TArray<FPotionRecipe>& GetAllRecipes() const { return Recipes; }

    // 재고
    UFUNCTION(BlueprintCallable, Category="Alchemy|Stock")
    void InitializeStock(FName PotionName);

    UFUNCTION(BlueprintCallable, Category="Alchemy|Stock")
    bool HasPotion(FName PotionName) const;

    UFUNCTION(BlueprintCallable, Category="Alchemy|Stock")
    int32 GetPotionCount(FName PotionName) const;

    UFUNCTION(BlueprintCallable, Category="Alchemy|Stock")
    bool DispensePotion(FName PotionName);

    UFUNCTION(BlueprintCallable, Category="Alchemy|Stock")
    void ReturnPotion(FName PotionName);

    UFUNCTION(BlueprintCallable, Category="Alchemy|Stock")
    void GetDispensableByIngredient(const FString& Ingredient, TMap<FName,int32>& OutList) const;

private:
    UPROPERTY()
    TArray<FPotionRecipe> Recipes;

    UPROPERTY()
    TMap<FName, int32> PotionStock;

    static constexpr int32 MAX_STOCK = 3;
};

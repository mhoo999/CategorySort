// © 2024 Hoon. A touch of creativity in every line of code.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCategorySortWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATEGORYSORT_API UMyCategorySortWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UMyCategorySortWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Sort(FString& InSortCondition);

protected:
	UPROPERTY()
	TSubclassOf<class UMyListRowWidget> MyListRowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ColumnContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMyButton> Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMyButton> Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMyButton> Strength;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMyButton> Intelligence;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMyButton> Vitality;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> ListView;

public:
	void GenerateRows();

};

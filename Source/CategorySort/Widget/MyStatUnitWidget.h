// © 2024 Hoon. A touch of creativity in every line of code.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStatUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATEGORYSORT_API UMyStatUnitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void InputMinValue(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void InputMaxValue(const FText& Text, ETextCommit::Type CommitMethod);

	void ClearText();


protected:
	UPROPERTY()
	TObjectPtr<class UObject> ParentWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> MinValueInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> MaxValueInput;

	int32 MinValue;
	int32 MaxValue;

public:
	void SetParentWidget(UObject* Object);

	void SetMinValue(int32 Value);
	void SetMaxValue(int32 Value);

};

// © 2024 Hoon. A touch of creativity in every line of code.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MyButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickStringParamDelegate, FString&);
/**
 * 
 */
UCLASS()
class CATEGORYSORT_API UMyButton : public UButton
{
	GENERATED_BODY()
	
public:
	FOnClickStringParamDelegate OnClickStringParamDelegate;

	UFUNCTION()
	void OnClick();

	void AddOnClickEvent();

private:
	FString StringParameter1;

public:
	void SetStringParameter1(const FString& InString) { StringParameter1 = InString; }

};

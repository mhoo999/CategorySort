// © 2024 Hoon. A touch of creativity in every line of code.


#include "Widget/MyStatUnitWidget.h"
#include "Widget/MyStatPopupWidget.h"

#include "Components/EditableText.h"

void UMyStatUnitWidget::NativeConstruct()
{
	MinValueInput->OnTextCommitted.AddDynamic(this, &UMyStatUnitWidget::InputMinValue);
	MaxValueInput->OnTextCommitted.AddDynamic(this, &UMyStatUnitWidget::InputMaxValue);
}

void UMyStatUnitWidget::InputMinValue(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (Text.ToString() == "")
	{
		return;
	}

	if (Text.IsNumeric())
	{
		int32 Value = FCString::Atoi(*Text.ToString());
		Value > 9 || Value < 1 ? MinValue = 1 : MinValue = Value;
	}
	else
	{
		MinValue = 1;
	}

	Cast<UMyStatPopupWidget>(ParentWidget)->ChangedValue(GetName(), FVector2D(MinValue, MaxValue));

}

void UMyStatUnitWidget::InputMaxValue(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (Text.ToString() == "")
	{
		return;
	}

	if (Text.IsNumeric())
	{
		int32 Value = FCString::Atoi(*Text.ToString());
		Value > 9 || Value < 1 ? MaxValue = 9 : MaxValue = Value;
	}
	else
	{
		MaxValue = 9;
	}

	Cast<UMyStatPopupWidget>(ParentWidget)->ChangedValue(GetName(), FVector2D(MinValue, MaxValue));
}

void UMyStatUnitWidget::ClearText()
{
	MinValueInput->SetText(FText::FromString(TEXT("")));
	MaxValueInput->SetText(FText::FromString(TEXT("")));
}

void UMyStatUnitWidget::SetParentWidget(UObject* Object)
{
	ParentWidget = Object;
}

void UMyStatUnitWidget::SetMinValue(int32 Value)
{
	MinValueInput->SetText(FText::FromString(FString::FromInt(Value)));
}

void UMyStatUnitWidget::SetMaxValue(int32 Value)
{
	MaxValueInput->SetText(FText::FromString(FString::FromInt(Value)));
}

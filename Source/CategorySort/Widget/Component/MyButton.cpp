// © 2024 Hoon. A touch of creativity in every line of code.


#include "MyButton.h"

#include "Styling/SlateTypes.h"


void UMyButton::OnClick()
{
	OnClickStringParamDelegate.ExecuteIfBound(StringParameter1);
}

void UMyButton::AddOnClickEvent()
{
	if (!OnClicked.IsBound())
	{
		OnClicked.AddDynamic(this, &UMyButton::OnClick);
	}
}

void UMyButton::SetEnable()
{
	FButtonStyle ButtonStyle = GetStyle();
	ButtonStyle.Normal.TintColor = FSlateColor(ACTIVATE_COL);
	SetStyle(ButtonStyle);
}

void UMyButton::SetDisable()
{
	FButtonStyle ButtonStyle = GetStyle();
	ButtonStyle.Normal.TintColor = FSlateColor(DEACTIVATE_COL);
	SetStyle(ButtonStyle);
}

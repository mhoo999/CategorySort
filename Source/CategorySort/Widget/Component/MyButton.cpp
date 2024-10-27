// © 2024 Hoon. A touch of creativity in every line of code.


#include "Widget/Component/MyButton.h"

#include "Components/Button.h"

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

// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInteractionWidget.h"

void UCerberusInteractionWidget::UpdateInteractionWidget(UCerberusInteractionComponent* InteractionComponent)
{
	OwningInteractionComponent = InteractionComponent;
	OnUpdateInteractionWidget();
}

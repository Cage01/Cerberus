// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInventoryItemWidget.h"

bool UCerberusInventoryItemWidget::SetItem(UCerberusItem* NewItem)
{
	if (NewItem && !Item)
	{
		Item = NewItem;
		return true;
	}

	return false;
}

﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPlayerController.h"

#include "Cerberus/Structs/CerberusNotification.h"

ACerberusPlayerController::ACerberusPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

ACerberusPlayerState* ACerberusPlayerController::GetCerberusPlayerState() const
{
	return CastChecked<ACerberusPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UCerberusAbilitySystemComponent* ACerberusPlayerController::GetCerberusAbilitySystemComponent() const
{
	const ACerberusPlayerState* CerberusPS = GetCerberusPlayerState();
	return (CerberusPS ? CerberusPS->GetCerberusAbilitySystemComponent() : nullptr);
}

void ACerberusPlayerController::ClientShowNotification_Implementation(FNotification Notification)
{
	ShowNotification(Notification);
}


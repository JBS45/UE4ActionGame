// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewMonster.h"
#include "../NewMonsterComponents/NewBossrStatus.h"

AMyNewMonster::AMyNewMonster() {
	StatusManager = CreateDefaultSubobject<UNewMonsterStatus>("StatusManager");
}

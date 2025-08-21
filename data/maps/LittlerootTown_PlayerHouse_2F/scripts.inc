LittlerootTown_PlayerHouse_2F_MapScripts::
	.byte 0

PlayerHouse2F_EventScript_CheckNewGame::
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 0, PlayerHouse2F_EventScript_SetIntroFlags
	end

PlayerHouse2F_EventScript_SetIntroFlags::
	lockall
	setvar VAR_LITTLEROOT_INTRO_STATE, 7
	
	releaseall
	end

// ASF (Additional Server Functions) examples - by noiseXYZ
// https://github.com/noiseXYZ/openmp-asf

#include <open.mp>
#include <Pawn.RakNet>
#include <asf>

main()
{
	printf("ASF %s by %s loaded", ASF_VERSION, ASF_AUTHOR);
}

// 1 = paused (ESC / alt-tab), 0 = back
public OnPlayerPauseStateChange(playerid, pausestate)
{
	if(pausestate) SetPlayerChatBubble(playerid, "AFK", 0xFF8080FF, 30.0, 60000);
	else SetPlayerChatBubble(playerid, " ", 0, 1.0, 1);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	// everyone else sees you as "Unknown" with skin 1
	if(!strcmp(cmdtext, "/disguise", true))
	{
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(i == playerid || !IsPlayerConnected(i)) continue;
			SetPlayerNameForPlayer(i, playerid, "Unknown");
			SetPlayerSkinForPlayer(i, playerid, 1);
		}
		SendClientMessage(playerid, -1, "You are disguised. /undisguise to stop.");
		return 1;
	}
	if(!strcmp(cmdtext, "/undisguise", true))
	{
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(i == playerid || !IsPlayerConnected(i)) continue;
			ResetPlayerNameForPlayer(i, playerid);
			ResetPlayerSkinForPlayer(i, playerid);
		}
		return 1;
	}
	// invisible for everyone else (sync is dropped too, not just the model)
	if(!strcmp(cmdtext, "/ghost", true))
	{
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(i == playerid || !IsPlayerConnected(i)) continue;
			if(IsPlayerHiddenForPlayer(i, playerid)) ShowPlayerForPlayer(i, playerid);
			else HidePlayerForPlayer(i, playerid);
		}
		return 1;
	}
	// no sprinting or jumping: the keys are removed before the server sees them
	if(!strcmp(cmdtext, "/nosprint", true))
	{
		SetPlayerDisabledKeysSync(playerid, KEY_SPRINT | KEY_JUMP);
		return 1;
	}
	// show 999 ping on the scoreboard
	if(!strcmp(cmdtext, "/fakeping", true))
	{
		TogglePlayerFakePing(playerid, true);
		SetPlayerFakePing(playerid, 999);
		return 1;
	}
	return 0;
}

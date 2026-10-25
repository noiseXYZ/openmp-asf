# openmp-asf

**ASF - Additional Server Functions for open.mp**
Author: **[noiseXYZ](https://github.com/noiseXYZ)**

ASF is basically **YSF for open.mp**. It gives open.mp servers the YSF functions that open.mp does not have natively, with the **same function names and parameters as YSF**, so most YSF code compiles unchanged.

> **Why "ASF" and not YSF?** It is YSF in all but name. The different name is only because I didn't want copyright issues with the YSF name. ASF contains no YSF source code: it is a new implementation of YSF's function list, written from scratch on top of Pawn.RakNet.

Made by **noiseXYZ**.

---

## Why ASF exists

YSF works by patching the memory of the SA-MP server binary. open.mp is a different server, so YSF cannot load on it. open.mp already added most of YSF's getters as natives, but everything YSF did with packets (per-player names and skins, hiding players and objects, fake sync, fake ping, and so on) was missing.

ASF rebuilds those functions as a plain Pawn include. It rewrites, blocks and sends network packets through [Pawn.RakNet](https://github.com/katursis/Pawn.RakNet), so no extra plugin is needed besides Pawn.RakNet.

## Requirements

- open.mp server
- Pawn.RakNet 1.5 or newer (open.mp component or plugin), with its include `Pawn.RakNet.inc`
- open.mp compiler (pawncc 3.10.x) with the open.mp includes

## Installation

1. Copy `asf.inc` into your `pawno/include` or `qawno/include` folder.
2. Include it after `<open.mp>` and `<Pawn.RakNet>`:

```pawn
#include <open.mp>
#include <Pawn.RakNet>
#include <asf>
```

3. If you use `pause.inc`, remove it. ASF has its own pause tracking with the YSF names (`IsPlayerPaused`, `GetPlayerPausedTime`, `OnPlayerPauseStateChange`). If `pause.inc` is still included, ASF skips its pause functions and `pause.inc` keeps providing them.
4. Remove `YSF.inc` if it is in your script. ASF stops with an error if both are included.

## Configuration

Define these before `#include <asf>` to change them:

| Define | Default | Meaning |
|---|---|---|
| `ASF_MAX_OVERRIDES` | 1024 | per-player skin/team/name/style/position overrides in total |
| `ASF_MAX_REMOVED` | 8192 | `RemoveBuildingForPlayer` calls remembered for `IsBuildingRemovedForPlayer` |
| `ASF_MAX_PICKUP_OVR` | 512 | `SetPickupForPlayer` overrides in total |
| `ASF_AFK_ACCURACY` | 1500 | ms without any sync before a player counts as paused |
| `ASF_NO_EXCLUSIVE_BROADCAST` | (off) | leave out `SetExclusiveBroadcast` and ASF's `OnOutgoingRPC` hook |

`ASF_VERSION` and `ASF_AUTHOR` are defined by the include.

## Functions

### Per-player view (what one player sees of another)

| Function | What it does |
|---|---|
| `SetPlayerTeamForPlayer(playerid, teamplayerid, teamid)` / `GetPlayerTeamForPlayer` | team of `teamplayerid` as seen by `playerid` |
| `SetPlayerSkinForPlayer(playerid, skinplayerid, skin)` / `GetPlayerSkinForPlayer` | skin seen by one player |
| `SetPlayerNameForPlayer(playerid, nameplayerid, const name[])` / `GetPlayerNameForPlayer` | name tag (and chat name client side) seen by one player |
| `SetPlayerFightStyleForPlayer(playerid, styleplayerid, style)` / `GetPlayerFightStyleForPlayer` | fighting style seen by one player |
| `SetPlayerPosForPlayer(playerid, posplayerid, x, y, z, forcesync = true)` | fake position for one viewer (on foot) |
| `SetPlayerRotationQuatForPlayer(playerid, quatplayerid, w, x, y, z, forcesync = true)` | fake rotation for one viewer (on foot) |
| `ApplyAnimationForPlayer(playerid, animplayerid, lib[], name[], delta, loop, lockx, locky, freeze, time)` | animation only one player sees |
| `SetPlayerChatBubbleForPlayer(forplayerid, playerid, text[], color, drawdistance, expiretime)` | chat bubble only one player sees |
| `ResetPlayerMarkerForPlayer(playerid, resetplayerid)` | resets a `SetPlayerMarkerForPlayer` colour |
| `HidePlayerForPlayer(forplayerid, playerid)` / `ShowPlayerForPlayer(forplayerid, playerid)` | make a player invisible for one viewer (sync is dropped too) |
| `AddPlayerForPlayer(forplayerid, playerid, isnpc = 0)` / `RemovePlayerForPlayer(forplayerid, playerid)` | add or remove a player from one viewer's player list |
| `SpawnForWorld(playerid)` | streams the player in again for everyone |
| `SendPlayerDeath(playerid, forplayerid = -1)` / `BroadcastDeath(playerid)` | show a death animation to others |
| `SendBulletData(senderid, forplayerid = -1, weaponid, hittype, hitid, origin xyz, target xyz, center xyz)` | send a fake bullet |

ASF extras (not in YSF): `ResetPlayerTeamForPlayer`, `ResetPlayerSkinForPlayer`, `ResetPlayerNameForPlayer`, `ResetPlayerFightStyleForPlayer`, `ResetPlayerPosForPlayer`, `ResetPlayerRotationQuatForPlayer`, `IsPlayerHiddenForPlayer`.

### Sync

| Function | What it does |
|---|---|
| `SetPlayerDisabledKeysSync(playerid, keys, updown = 0, leftright = 0)` / `GetPlayerDisabledKeysSync` | removes keys from the player's sync before the server and other players see them |
| `GetPlayerLastSyncedVehicleID(playerid)` / `GetPlayerLastSyncedTrailerID(playerid)` | last vehicle / trailer from driver sync |
| `GetPlayerSyncWeapon(playerid)` | weapon from the last sync |
| `SetPlayerSyncKeys`, `SetPlayerSyncPos`, `SetPlayerSyncPosition`, `SetPlayerSyncVelocity`, `SetPlayerSyncRotationQuat`, `SetPlayerSyncSpecialAction`, `SetPlayerSyncHealth`, `SetPlayerSyncArmour`, `SetPlayerSyncWeapon`, `SetPlayerSyncWeaponState`, `SetPlayerSyncCameraFrontVector`, `SetPlayerSyncCameraPos`, `SetPlayerSyncCameraMode`, `SetPlayerSyncVehicleId`, `SetPlayerSyncVehicleSeat`, `SetPlayerSyncVehiclePosition`, `SetPlayerSyncVehicleVelocity`, `SetPlayerSyncVehicleRotQuat`, `SetPlayerSyncVehicleHealth`, `SetPlayerSyncTrainSpeed` | change the player's last sync |
| `UpdatePlayerSyncData(playerid, type = -1, setstate = false)` | sends the (changed) last sync to everyone who streams the player |

`type` for `UpdatePlayerSyncData`: `ASF_SYNC_AUTO` (-1, by player state), `ASF_SYNC_ONFOOT`, `ASF_SYNC_INCAR`, `ASF_SYNC_PASSENGER`, `ASF_SYNC_AIM`.

### Objects, vehicles, pickups, buildings

| Function | What it does |
|---|---|
| `HideObjectForPlayer(playerid, objectid)` / `ShowObjectForPlayer(playerid, objectid)` | hide a global object for one player |
| `HideNewObjectsForPlayer(playerid, toggle)` / `NewObjectsHiddenForPlayer(playerid)` | hide every global object sent to the player from now on |
| `IsObjectHiddenForPlayer(playerid, objectid)` | |
| `HideVehicle(vehicleid)` / `ShowVehicle(vehicleid)` / `IsVehicleHidden(vehicleid)` | hide a vehicle for everyone |
| `SetPickupForPlayer(playerid, id, model, type, x, y, z)` | change how a global pickup looks for one player |
| `IsBuildingRemovedForPlayer(playerid, modelid, x, y, z, radius)` | ASF records every `RemoveBuildingForPlayer` call to answer this |

### Network and server

| Function | What it does |
|---|---|
| `SetExclusiveBroadcast(toggle)` / `BroadcastToPlayer(playerid, toggle = 1)` | while on, RPCs only go to the players you picked |
| `TogglePlayerScoresPingsUpdate(playerid, toggle)` | stop scoreboard updates for a player |
| `TogglePlayerFakePing(playerid, toggle)` / `SetPlayerFakePing(playerid, ping)` | show a fake ping on everyone's scoreboard |
| `SendRPC(playerid, rpcid, ...)` / `SendData(playerid, ...)` | send a raw RPC / packet from `BS_*` type + value pairs, `playerid` -1 = everyone |
| `LoadFilterScript(name[])` / `UnLoadFilterScript(name[])` | through the `loadfs` / `unloadfs` RCON commands |

`SendRPC` / `SendData` types: `BS_BOOL`, `BS_CHAR`, `BS_UNSIGNEDCHAR`, `BS_SHORT`, `BS_UNSIGNEDSHORT`, `BS_INT`, `BS_UNSIGNEDINT`, `BS_FLOAT`, `BS_STRING` (characters only: write the length first yourself if the RPC needs it).

### Pause

| Function | What it does |
|---|---|
| `IsPlayerPaused(playerid)` | true after `ASF_AFK_ACCURACY` ms without sync (ESC menu, alt-tab) |
| `GetPlayerPausedTime(playerid)` | how long the player has been paused, **in milliseconds** |
| `YSF_SetAFKAccuracy(time_ms)` / `YSF_GetAFKAccuracy()` | change the threshold at runtime |

Players in class selection or dead never count as paused.

### Callbacks

```pawn
public OnPlayerPauseStateChange(playerid, pausestate) {}          // 1 = paused, 0 = back
public OnPlayerStatsAndWeaponsUpdate(playerid) { return 1; }      // return 0 = drop the update
public OnOutcomeScmEvent(playerid, issuerid, E_SCM_EVENT_TYPE:eventid, vehicleid, arg1, arg2) { return 1; } // return 0 = block
```

## Example

```pawn
#include <open.mp>
#include <Pawn.RakNet>
#include <asf>

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(!strcmp(cmdtext, "/disguise", true))
	{
		// everyone else sees this player as "Unknown" in skin 1
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(i == playerid || !IsPlayerConnected(i)) continue;
			SetPlayerNameForPlayer(i, playerid, "Unknown");
			SetPlayerSkinForPlayer(i, playerid, 1);
		}
		return 1;
	}
	return 0;
}
```

More in [examples/example.pwn](examples/example.pwn). [tests/compile_test.pwn](tests/compile_test.pwn) calls every function once and is used as the compile check.

## Every YSF function and its status

Of the 347 functions and callbacks in `YSF.inc`:
- about 220 are **already native in open.mp**
- about 75 are **added by ASF**
- about 50 **can't be done from Pawn**

### Already native in open.mp

You don't need ASF for these, open.mp has them:
- **Server, names, bans:** `AddServerRule`, `SetServerRule`, `IsValidServerRule`, `AllowNickNameCharacter`, `IsNickNameCharacterAllowed`, `IsValidNickName`, `SetModeRestartTime`, `GetModeRestartTime`, `GetRunningTimers`, `ClearBanList`, `IsBanned`, `ToggleChatTextReplacement`, `ChatTextReplacementToggled`, `execute`.
- **Formatted messages** (the normal natives take format arguments): `SendClientMessagef`, `SendClientMessageToAllf`, `SendPlayerMessageToPlayerf`, `SendPlayerMessageToAllf`, `GameTextForPlayerf`, `GameTextForAllf`, `SendRconCommandf`.
- **Player:** `GetPlayerGravity`, `SetPlayerGravity`, `GetGravity`, `SetPlayerAdmin`, `GetPlayerRawIp`, `GetPlayerZAim`, `GetPlayerRotationQuat`, `GetPlayerWeather`, `GetPlayerSkillLevel`, `GetPlayerSirenState`, `GetPlayerLandingGearState`, `GetPlayerHydraReactorAngle`, `GetPlayerTrainSpeed`, `GetPlayerSurfingOffsets`, `GetPlayerSurfingPlayerObjectID`, `GetPlayerCameraTargetPlayerObj`, `IsPlayerCameraTargetEnabled`, `GetPlayerAttachedObject`, `GetPlayerBuildingsRemoved`, `GetPlayerDialogID`, `GetPlayerSpectateID`, `GetPlayerSpectateType`, `GetPlayerCheckpoint`, `GetPlayerRaceCheckpoint`, `IsPlayerCheckpointActive`, `IsPlayerRaceCheckpointActive`, `IsPlayerControllable`, `IsPlayerSpawned`, `IsPlayerInModShop`, `TogglePlayerWidescreen`, `IsPlayerWidescreenToggled`, `TogglePlayerGhostMode`, `GetPlayerGhostMode`, `GetPlayerConnectMode`, `GetSpawnInfo`, `GetPlayerClass`, `EditPlayerClass`, `GetAvailableClasses`.
- **Vehicles:** `GetVehicleSpawnInfo`, `SetVehicleSpawnInfo`, `GetVehicleColor`, `GetVehiclePaintjob`, `GetVehicleInterior`, `GetVehicleNumberPlate`, `GetVehicleRespawnDelay`, `SetVehicleRespawnDelay`, `GetVehicleRespawnTick`, `GetVehicleOccupiedTick`, `HasVehicleBeenOccupied`, `IsVehicleOccupied`, `IsVehicleDead`, `GetVehicleLastDriver`, `GetVehicleCab`, `GetVehicleMatrix`, `GetVehicleModelCount`, `GetVehicleModelsUsed`, `IsValidVehicle`, `IsVehicleSirenEnabled`, `ToggleVehicleSirenEnabled`, `SetVehicleParamsSirenState`.
- **Objects:** `GetObjectDrawDistance`, `GetObjectMoveSpeed`, `SetObjectMoveSpeed`, `GetObjectTarget`, `GetObjectAttachedData`, `GetObjectAttachedOffset`, `GetObjectSyncRotation`, `GetObjectMaterial`, `GetObjectMaterialText`, `IsObjectMaterialSlotUsed`, `IsObjectNoCameraCol`, `GetObjectType`, the matching `GetPlayerObject*` / `IsPlayerObject*` functions, `SetPlayerObjectMoveSpeed`, `AttachPlayerObjectToObject`.
- **Pickups:** `CreatePlayerPickup`, `DestroyPlayerPickup`, `IsValidPickup`, `IsValidPlayerPickup`, `GetPickupModel`, `SetPickupModel`, `GetPickupPos`, `SetPickupPos`, `GetPickupType`, `SetPickupType`, `GetPickupVirtualWorld`, `SetPickupVirtualWorld`, the `GetPlayerPickup*` functions, `ShowPickupForPlayer`, `HidePickupForPlayer`, `IsPickupStreamedIn`, `IsPlayerPickupStreamedIn`, `OnPickupStreamIn`, `OnPickupStreamOut`, `OnPlayerPickUpPlayerPickup`.
- **Gang zones:** `CreatePlayerGangZone`, `PlayerGangZoneShow`, `PlayerGangZoneHide`, `PlayerGangZoneFlash`, `PlayerGangZoneStopFlash`, `PlayerGangZoneDestroy`, `PlayerGangZoneGetColor`, `PlayerGangZoneGetFlashColor`, `PlayerGangZoneGetPos`, `IsPlayerGangZoneVisible`, `IsPlayerGangZoneFlashing`, `GangZoneGetPos`, `GangZoneGetColorForPlayer`, `GangZoneGetFlashColorForPlayer`, `IsGangZoneVisibleForPlayer`, `IsGangZoneFlashingForPlayer`, `IsPlayerInGangZone`, `IsPlayerInPlayerGangZone`, `IsValidGangZone`, `IsValidPlayerGangZone`, `OnPlayerEnterGangZone`, `OnPlayerLeaveGangZone`, `OnPlayerEnterPlayerGangZone`, `OnPlayerLeavePlayerGangZone`.
- **Textdraws:** every `TextDrawGet*` / `TextDrawIs*` and `PlayerTextDrawGet*` / `PlayerTextDrawIs*` function, `TextDrawSetPos`, `PlayerTextDrawSetPos`, `TextDrawSetStringForPlayer`, `IsTextDrawVisibleForPlayer`, `IsPlayerTextDrawVisible`, `IsValidTextDraw`, `IsValidPlayerTextDraw`.
- **3D labels, menus, actors, weapons:** every `Get3DTextLabel*` / `GetPlayer3DTextLabel*` function, `Is3DTextLabelStreamedIn`, `IsValid3DTextLabel`, `IsValidPlayer3DTextLabel`, `GetMenuColumnHeader`, `GetMenuColumnWidth`, `GetMenuColumns`, `GetMenuItem`, `GetMenuItems`, `GetMenuPos`, `IsMenuDisabled`, `IsMenuRowDisabled`, `GetActorSkin`, `GetActorAnimation`, `GetActorSpawnInfo`, `GetWeaponSlot`.

### Added by ASF

`SetPlayerTeamForPlayer`, `GetPlayerTeamForPlayer`, `SetPlayerSkinForPlayer`, `GetPlayerSkinForPlayer`, `SetPlayerNameForPlayer`, `GetPlayerNameForPlayer`, `SetPlayerFightStyleForPlayer`, `GetPlayerFightStyleForPlayer`, `SetPlayerPosForPlayer`, `SetPlayerRotationQuatForPlayer`, `ApplyAnimationForPlayer` (YSF marked it "doesn't work", ASF's works), `SetPlayerChatBubbleForPlayer`, `ResetPlayerMarkerForPlayer`, `ShowPlayerForPlayer`, `HidePlayerForPlayer`, `AddPlayerForPlayer`, `RemovePlayerForPlayer`, `SpawnForWorld`, `SendPlayerDeath`, `BroadcastDeath`, `SendBulletData`, `SetPlayerDisabledKeysSync`, `GetPlayerDisabledKeysSync`, `GetPlayerLastSyncedVehicleID`, `GetPlayerLastSyncedTrailerID`, `GetPlayerSyncWeapon`, `SetPlayerSyncKeys`, `SetPlayerSyncPos`, `SetPlayerSyncPosition`, `SetPlayerSyncVelocity`, `SetPlayerSyncRotationQuat`, `SetPlayerSyncCameraFrontVector`, `SetPlayerSyncCameraPos`, `SetPlayerSyncCameraMode`, `SetPlayerSyncWeapon`, `SetPlayerSyncWeaponState`, `SetPlayerSyncSpecialAction`, `SetPlayerSyncHealth`, `SetPlayerSyncArmour`, `SetPlayerSyncVehicleId`, `SetPlayerSyncVehicleSeat`, `SetPlayerSyncVehiclePosition`, `SetPlayerSyncVehicleVelocity`, `SetPlayerSyncVehicleRotQuat`, `SetPlayerSyncVehicleHealth`, `SetPlayerSyncTrainSpeed`, `UpdatePlayerSyncData`, `HideObjectForPlayer`, `ShowObjectForPlayer`, `HideNewObjectsForPlayer`, `IsObjectHiddenForPlayer`, `NewObjectsHiddenForPlayer`, `HideVehicle`, `ShowVehicle`, `IsVehicleHidden`, `SetPickupForPlayer`, `IsBuildingRemovedForPlayer`, `SetExclusiveBroadcast`, `BroadcastToPlayer`, `TogglePlayerScoresPingsUpdate`, `TogglePlayerFakePing`, `SetPlayerFakePing`, `SendRPC`, `SendData`, `LoadFilterScript`, `UnLoadFilterScript`, `IsPlayerPaused`, `GetPlayerPausedTime`, `YSF_SetAFKAccuracy`, `YSF_GetAFKAccuracy`, `OnPlayerPauseStateChange`, `OnPlayerStatsAndWeaponsUpdate`, `OnOutcomeScmEvent`.

### Can't be done from Pawn, and why

ASF is a Pawn include, so it can only work with what Pawn and Pawn.RakNet can reach. These need more:

| Functions | Reason |
|---|---|
| `SetMaxPlayers`, `SetMaxNPCs` | open.mp creates its player pools at a fixed size at startup |
| `SetPlayerConnectMode`, `SetTimeoutTime`, `ToggleCloseConnectionFix`, `SetSyncBounds`, `GetSyncBounds` | need RakNet connection internals or the server's own sync checks |
| `SetVehicleBeenOccupied`, `SetVehicleOccupiedTick`, `SetVehicleRespawnTick`, `SetVehicleDead` | values stored inside the server: open.mp natives can read them, nothing can write them from Pawn |
| `SetPlayerNameInServerQuery`, `GetPlayerNameInServerQuery`, `ResetPlayerNameInServerQuery`, `OnServerQueryInfo`, `GetServerRuleFlags`, `SetServerRuleFlags` | server browser queries are answered inside open.mp's query code; Pawn.RakNet never sees them |
| `OnServerMessage`, `YSF_ToggleOnServerMessage`, `YSF_IsOnServerMessageEnabled`, `EnableConsoleMSGsForPlayer`, `DisableConsoleMSGsForPlayer`, `HasPlayerConsoleMessages` | need a hook on the console / log output |
| `OnRemoteRCONPacket`, `ChangeRCONCommandName`, `GetRCONCommandName`, `OnSystemCommandExecute` | need hooks inside RCON and the console's command handling |
| `GetFilterScriptCount`, `GetFilterScriptName`, `CallFunctionInScript` | Pawn can't list the other loaded scripts (use `CallRemoteFunction` instead) |
| `GetServerSettings`, `YSF_SetTickRate`, `YSF_GetTickRate`, `YSF_SetExtendedNetStatsEnabled`, `YSF_IsExtendedNetStatsEnabled`, `YSF_EnableNightVisionFix`, `YSF_IsNightVisionFixEnabled` | switches for YSF's own SA-MP memory patches; there is nothing to switch on open.mp (open.mp has its own network stats natives) |
| `GetNPCCommandLine`, `SetPlayerVersion`, `GetLocalIP`, `GetRecordingDirectory`, `SetRecordingDirectory` | internal values no native or packet exposes |
| `GetColCount`, `GetColSphereRadius`, `GetColSphereOffset` | need the game's collision data (ColAndreas has model bounding data) |
| `OnPlayerClientGameInit`, `SendPlayerClientGameInit` | possible by rewriting the game-init RPC, left out until its open.mp layout is checked: a wrong layout stops players from joining |
| `ffind`, `frename`, `dfind`, `dcreate`, `drename` | open.mp has no directory functions (use pawn-fsutil or filemanager) |

Most of the last table is possible with a C++ open.mp component (the open.mp SDK can see the scripts, the console and the RakNet server). That may come as a companion `asf` component later.

## Differences from YSF

- **Overrides stay until you reset them.** `SetPlayerPosForPlayer`, the `...ForPlayer` overrides and hidden players stay active until the matching `Reset...ForPlayer` / `Show...` call or a disconnect.
- **`GetPlayerPausedTime` is in milliseconds** (`pause.inc` used seconds).
- **`setstate` in `UpdatePlayerSyncData` and `SetPlayerSyncVehicleId` is ignored**: the server's real player state can't be changed from Pawn.
- **`SetPlayerSync*` change only what is sent**, not what the server's own getters return. The player's next real sync replaces them.
- **Objects:** `HideObjectForPlayer` works on global objects only (streamer objects are player objects; use streamer's per-player functions). `ShowObjectForPlayer` restores model, materials, position and rotation, not a `MoveObject` that is still running. `HideNewObjectsForPlayer` hides every global object created for the player while it is on, including the ones sent when they connect.
- **`SetExclusiveBroadcast`** limits RPCs only, not sync packets.
- **`SendRPC` / `SendData` `BS_STRING`** writes the characters without a length.

## How it works

ASF registers Pawn.RakNet handlers for the sync packets and RPCs it needs:

- **Incoming sync (on foot, driver, passenger, aim, spectating):** ASF stores the last sync, tracks pause state, applies disabled keys and records the last vehicle.
- **Outgoing sync:** ASF drops packets of hidden players and vehicles, and rewrites positions and rotations per viewer.
- **Outgoing RPCs (stream-in, name, skin, team, fighting style, objects, vehicles, pickups, scoreboard):** ASF applies per-viewer overrides and blocks hidden items.

ASF hooks `OnPlayerConnect`, `OnPlayerDisconnect`, `OnOutgoingRPC` and `RemoveBuildingForPlayer` with the usual ALS chaining, so your own callbacks keep working.

## Status

Version **1.0.0**. It compiles cleanly with the open.mp compiler, but not every function has been tested on a live server yet. The packet layouts follow the SA-MP 0.3.7 protocol that open.mp speaks (DL clients are detected). If something behaves differently in game, open an issue.

## Author and credits

- **ASF by [noiseXYZ](https://github.com/noiseXYZ).** Author and maintainer.
- The function list and names follow **YSF** by kurta999 and contributors, so YSF scripts work with ASF. No YSF code is used.
- Built on **[Pawn.RakNet](https://github.com/katursis/Pawn.RakNet)** by katursis.

## License

MIT, Copyright (c) 2026 noiseXYZ. See [LICENSE](LICENSE).

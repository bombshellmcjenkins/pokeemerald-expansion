#include "global.h"
#include "malloc.h"
#include "battle.h"
#include "pokemon.h"
#include "battle_ai_main.h"
#include "battle_ai_util.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "battle_z_move.h"
#include "battle_birthright.h"
#include "battle_scripts.h"
#include "graphics.h"
#include "sprite.h"
#include "window.h"
#include "string_util.h"
#include "text.h"
#include "item.h"
#include "strings.h"
#include "sound.h"
#include "constants/songs.h"
#include "decompress.h"
#include "task.h"
#include "util.h"
#include "gpu_regs.h"
#include "battle_message.h"
#include "pokedex.h"
#include "palette.h"
#include "international_string_util.h"
#include "safari_zone.h"
#include "battle_anim.h"
#include "constants/battle_anim.h"
#include "constants/rgb.h"
#include "battle_debug.h"
#include "data.h"
#include "pokemon_summary_screen.h"
#include "constants/songs.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/hold_effects.h"
#include "constants/battle_string_ids.h"
#include "constants/battle_move_effects.h"
#include "constants/abilities.h"
#include "constants/moves.h"

// Function Declarations
static void BirthrightDisplayPpNumber(u32 battler);
static void BirthrightDisplayPower(u16 move);
static void BirthrightDisplayMoveType(u16 move);

static const u8 sText_PowerColon[] = _("Power: ");

bool32 CanUseBirthright(u32 battler)
{
    u32 holdEffect = GetBattlerHoldEffect(battler, FALSE);

	// Disable in weird scenarios (safari + catch tutorial)
    if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_WALLY_TUTORIAL))
        return FALSE;

	// Disable if battler is holding the Faded Ring
    if (holdEffect == HOLD_EFFECT_FADED_RING)
        return FALSE;

    return TRUE;	
}

void ActivateBirthright(u32 battler)
{
//	gBattleStruct->zmove.baseMoves[battler] = gBattleMons[battler].moves[gBattleStruct->chosenMovePositions[battler]];
	
	// Set the chosen move position to the innate move index
	gBattleStruct->chosenMovePositions[battler] = MAX_SELECTABLE_MOVES - 1;
    SetActiveGimmick(battler, GIMMICK_BIRTHRIGHT);
}

bool32 MoveSelectionDisplayBirthright(u32 battler)
{
	u32 i;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleResources->bufferA[battler][4]);
    u16 move = moveInfo->moves[MAX_SELECTABLE_MOVES - 1];

    PlaySE(SE_SELECT);
    gBattleStruct->viewingBirthright = TRUE;

    if (move != MOVE_NONE)
    {
        // Clear move slots
        for (i = 0; i < MAX_LEARNED_MOVES; ++i)
        {
            MoveSelectionDestroyCursorAt(i);
            StringCopy(gDisplayedStringBattle, gText_EmptyString2);
            BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
        }
        
        BirthrightDisplayPower(move);
        StringCopy(gDisplayedStringBattle, GetMoveName(move));
        
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_NAME_1);

        BirthrightDisplayPpNumber(battler);
        BirthrightDisplayMoveType(move);
        MoveSelectionCreateCursorAt(0, 0);
        return TRUE;
    }
	
	return FALSE;

}

static void BirthrightDisplayPower(u16 move)
{
    u8 *txtPtr;
    u16 power = gMovesInfo[move].power;

    if (gMovesInfo[move].category != DAMAGE_CATEGORY_STATUS)
    {
        txtPtr = StringCopy(gDisplayedStringBattle, sText_PowerColon);
        ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_NAME_3);
    }
}

static void BirthrightDisplayPpNumber(u32 battler)
{
    u8 *txtPtr;
	u8 currentPP = 1;
	u8 maxPP = 1;
//	u32 i;
	
    if (gBattleResources->bufferA[battler][2] == TRUE) // Check if we didn't want to display pp number
        return;

/*	
    for (i = 0; i < MAX_SELECTABLE_MOVES; ++i)
	{
		if (gBattleMons[battler].moves[i] == gSpeciesInfo[gBattleMons[battler].species].innateMove)
		{
			currentPP = gBattleMons[battler].pp[i];
			maxPP = gMovesInfo[gBattleMons[battler].moves[i]].pp;
		}
	}
*/
	
	currentPP = gBattleMons[battler].pp[MAX_SELECTABLE_MOVES - 1];
	maxPP = gMovesInfo[gBattleMons[battler].moves[MAX_SELECTABLE_MOVES - 1]].pp;
	
    SetPpNumbersPaletteInMoveSelection(battler);
//    txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, 1, STR_CONV_MODE_RIGHT_ALIGN, 2);
    txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, currentPP, STR_CONV_MODE_RIGHT_ALIGN, 2);
    *(txtPtr)++ = CHAR_SLASH;
 //   ConvertIntToDecimalStringN(txtPtr, 1, STR_CONV_MODE_RIGHT_ALIGN, 2);
    ConvertIntToDecimalStringN(txtPtr, maxPP, STR_CONV_MODE_RIGHT_ALIGN, 2);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_PP_REMAINING);
}

static void BirthrightDisplayMoveType(u16 move)
{
    u8 *txtPtr, *end;
    u32 moveType = GetMoveType(move);

    txtPtr = StringCopy(gDisplayedStringBattle, gText_MoveInterfaceType);
    *(txtPtr)++ = EXT_CTRL_CODE_BEGIN;
    *(txtPtr)++ = EXT_CTRL_CODE_FONT;
    *(txtPtr)++ = FONT_NORMAL;

    end = StringCopy(txtPtr, gTypesInfo[moveType].name);
    PrependFontIdToFit(txtPtr, end, FONT_NORMAL, WindowWidthPx(B_WIN_MOVE_TYPE) - 25);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_TYPE);
}


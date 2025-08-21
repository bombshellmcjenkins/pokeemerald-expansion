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

#define STAT_STAGE(battler, stat) (gBattleMons[battler].statStages[stat - 1])

// Function Declarations
static void ZMoveSelectionDisplayPpNumber(u32 battler);
static void ZMoveSelectionDisplayPower(u16 move, u16 zMove);
static void ZMoveSelectionDisplayMoveType(u16 zMove, u32 battler);

// Const Data
static const struct SignatureZMove sSignatureZMoves[] =
{
	/*
    {SPECIES_PIKACHU_COSPLAY,         ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},
    {SPECIES_PIKACHU_ROCK_STAR,       ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},
    {SPECIES_PIKACHU_BELLE,           ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},
    {SPECIES_PIKACHU_POP_STAR,        ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},
    {SPECIES_PIKACHU_PHD,             ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},
    {SPECIES_PIKACHU_LIBRE,           ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},

    {SPECIES_RAICHU_ALOLA,            ITEM_ALORAICHIUM_Z,        MOVE_THUNDERBOLT,         MOVE_STOKED_SPARKSURFER},
    {SPECIES_DECIDUEYE,               ITEM_DECIDIUM_Z,           MOVE_SPIRIT_SHACKLE,      MOVE_SINISTER_ARROW_RAID},
    {SPECIES_INCINEROAR,              ITEM_INCINIUM_Z,           MOVE_DARKEST_LARIAT,      MOVE_MALICIOUS_MOONSAULT},
    {SPECIES_KOMMO_O,                 ITEM_KOMMONIUM_Z,          MOVE_CLANGING_SCALES,     MOVE_CLANGOROUS_SOULBLAZE},
    {SPECIES_LUNALA,                  ITEM_LUNALIUM_Z,           MOVE_MOONGEIST_BEAM,      MOVE_MENACING_MOONRAZE_MAELSTROM},
    {SPECIES_NECROZMA_DAWN_WINGS,     ITEM_LUNALIUM_Z,           MOVE_MOONGEIST_BEAM,      MOVE_MENACING_MOONRAZE_MAELSTROM},
    {SPECIES_LYCANROC_MIDDAY,         ITEM_LYCANIUM_Z,           MOVE_STONE_EDGE,          MOVE_SPLINTERED_STORMSHARDS},
    {SPECIES_LYCANROC_MIDNIGHT,       ITEM_LYCANIUM_Z,           MOVE_STONE_EDGE,          MOVE_SPLINTERED_STORMSHARDS},
    {SPECIES_LYCANROC_DUSK,           ITEM_LYCANIUM_Z,           MOVE_STONE_EDGE,          MOVE_SPLINTERED_STORMSHARDS},
    {SPECIES_MARSHADOW,               ITEM_MARSHADIUM_Z,         MOVE_SPECTRAL_THIEF,      MOVE_SOUL_STEALING_7_STAR_STRIKE},
    {SPECIES_MIMIKYU_DISGUISED,       ITEM_MIMIKIUM_Z,           MOVE_PLAY_ROUGH,          MOVE_LETS_SNUGGLE_FOREVER},
    {SPECIES_MIMIKYU_BUSTED,          ITEM_MIMIKIUM_Z,           MOVE_PLAY_ROUGH,          MOVE_LETS_SNUGGLE_FOREVER},
    {SPECIES_MIMIKYU_TOTEM_DISGUISED, ITEM_MIMIKIUM_Z,           MOVE_PLAY_ROUGH,          MOVE_LETS_SNUGGLE_FOREVER},
    {SPECIES_MIMIKYU_BUSTED_TOTEM,    ITEM_MIMIKIUM_Z,           MOVE_PLAY_ROUGH,          MOVE_LETS_SNUGGLE_FOREVER},
    {SPECIES_PIKACHU_ORIGINAL,        ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_HOENN,           ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_SINNOH,          ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_UNOVA,           ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_KALOS,           ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_ALOLA,           ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_PARTNER,         ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PIKACHU_WORLD,           ITEM_PIKASHUNIUM_Z,        MOVE_THUNDERBOLT,         MOVE_10000000_VOLT_THUNDERBOLT},
    {SPECIES_PRIMARINA,               ITEM_PRIMARIUM_Z,          MOVE_SPARKLING_ARIA,      MOVE_OCEANIC_OPERETTA},
    {SPECIES_SOLGALEO,                ITEM_SOLGANIUM_Z,          MOVE_SUNSTEEL_STRIKE,     MOVE_SEARING_SUNRAZE_SMASH},
    {SPECIES_NECROZMA_DUSK_MANE,      ITEM_SOLGANIUM_Z,          MOVE_SUNSTEEL_STRIKE,     MOVE_SEARING_SUNRAZE_SMASH},
    {SPECIES_TAPU_KOKO,               ITEM_TAPUNIUM_Z,           MOVE_NATURES_MADNESS,     MOVE_GUARDIAN_OF_ALOLA},
    {SPECIES_TAPU_BULU,               ITEM_TAPUNIUM_Z,           MOVE_NATURES_MADNESS,     MOVE_GUARDIAN_OF_ALOLA},
    {SPECIES_TAPU_LELE,               ITEM_TAPUNIUM_Z,           MOVE_NATURES_MADNESS,     MOVE_GUARDIAN_OF_ALOLA},
    {SPECIES_TAPU_FINI,               ITEM_TAPUNIUM_Z,           MOVE_NATURES_MADNESS,     MOVE_GUARDIAN_OF_ALOLA},
    {SPECIES_NECROZMA_ULTRA,          ITEM_ULTRANECROZIUM_Z,     MOVE_PHOTON_GEYSER,       MOVE_LIGHT_THAT_BURNS_THE_SKY},
    {SPECIES_MEW,                     ITEM_MEWNIUM_Z,            MOVE_PSYCHIC,             MOVE_GENESIS_SUPERNOVA},
    {SPECIES_PIKACHU,                 ITEM_PIKANIUM_Z,           MOVE_VOLT_TACKLE,         MOVE_CATASTROPIKA},
    {SPECIES_EEVEE,                   ITEM_EEVIUM_Z,             MOVE_LAST_RESORT,         MOVE_EXTREME_EVOBOOST},
    {SPECIES_SNORLAX,                 ITEM_SNORLIUM_Z,           MOVE_GIGA_IMPACT,         MOVE_PULVERIZING_PANCAKE},
	*/
	{SPECIES_TREECKO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LEER},
	{SPECIES_GROVYLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TRAILBLAZE},
	{SPECIES_SCEPTILE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LEAF_BLADE},
	{SPECIES_TORCHIC,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PECK},
	{SPECIES_COMBUSKEN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DOUBLE_KICK},
	{SPECIES_BLAZIKEN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BLAZE_KICK},
	{SPECIES_MUDKIP,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MUD_SLAP},
	{SPECIES_MARSHTOMP,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MUD_SHOT},
	{SPECIES_SWAMPERT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WAVE_CRASH},
	{SPECIES_POOCHYENA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HOWL},
	{SPECIES_MIGHTYENA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CRUNCH},
	{SPECIES_ZIGZAGOON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ODOR_SLEUTH},
	{SPECIES_LINOONE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SLASH},
	{SPECIES_WURMPLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_STING},
	{SPECIES_SILCOON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LIFE_DEW},
	{SPECIES_BEAUTIFLY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DRAINING_KISS},
	{SPECIES_CASCOON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ENDURE},
	{SPECIES_DUSTOX,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TOXIC},
	{SPECIES_LOTAD,				  	  ITEM_NONE,				 MOVE_NONE,				   MOVE_ABSORB},
	{SPECIES_LOMBRE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BUBBLEBEAM},
	{SPECIES_LUDICOLO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ACCELERANDO},
	{SPECIES_SEEDOT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ASTONISH},
	{SPECIES_NUZLEAF,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ECHOED_VOICE},
	{SPECIES_SHIFTRY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PETAL_BLIZZARD},
	{SPECIES_TAILLOW,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ENDEAVOR},
	{SPECIES_SWELLOW,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ACROBATICS},
	{SPECIES_WINGULL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_GUST},
	{SPECIES_PELIPPER,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_STOCKPILE},
	{SPECIES_RALTS,				 	  ITEM_NONE,				 MOVE_NONE,				   MOVE_CHARM},
	{SPECIES_KIRLIA	,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TELEKINESIS},
	{SPECIES_GARDEVOIR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FUTURE_SIGHT},
	{SPECIES_SURSKIT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AQUA_JET},
	{SPECIES_MASQUERAIN,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_QUIVER_DANCE},
	{SPECIES_SHROOMISH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_POWDER},
	{SPECIES_BRELOOM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MACH_PUNCH},
	{SPECIES_SLAKOTH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SLACK_OFF},
	{SPECIES_VIGOROTH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_THRASH},
	{SPECIES_SLAKING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_COUNTER},
	{SPECIES_NINCADA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FURY_CUTTER},
	{SPECIES_NINJASK,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DOUBLE_TEAM},
	{SPECIES_SHEDINJA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SPIRIT_SHACKLE},
	{SPECIES_WHISMUR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_UPROAR},
	{SPECIES_LOUDRED,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HYPER_VOICE},
	{SPECIES_EXPLOUD,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HYPER_VOICE},
	{SPECIES_MAKUHITA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BULK_UP},
	{SPECIES_HARIYAMA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FORCE_PALM},
	{SPECIES_AZURILL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BOUNCE},
	{SPECIES_NOSEPASS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BLOCK},
	{SPECIES_SKITTY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CHARM},
	{SPECIES_DELCATTY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BATON_PASS},
	{SPECIES_SABLEYE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POWER_GEM},
	{SPECIES_MAWILE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SUPER_FANG},
	{SPECIES_ARON,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_HEADBUTT},
	{SPECIES_LAIRON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HEAVY_SLAM},
	{SPECIES_AGGRON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HEAVY_SLAM},
	{SPECIES_MEDITITE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MEDITATE},
	{SPECIES_MEDICHAM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DETECT},
	{SPECIES_ELECTRIKE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SPARK},
	{SPECIES_MANECTRIC,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DISCHARGE},
	{SPECIES_PLUSLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HELPING_HAND},
	{SPECIES_MINUN,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_HELPING_HAND},
	{SPECIES_VOLBEAT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TAIL_GLOW},
	{SPECIES_ILLUMISE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MOONLIGHT},
	{SPECIES_ROSELIA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MAGICAL_LEAF},
	{SPECIES_GULPIN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_GASTRO_ACID},
	{SPECIES_SWALOT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ACID_SPRAY},
	{SPECIES_CARVANHA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BITE},
	{SPECIES_SHARPEDO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FISHIOUS_REND},
	{SPECIES_WAILMER,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WATER_GUN},
	{SPECIES_WAILORD,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WATER_SPOUT},
	{SPECIES_NUMEL,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_AMNESIA},
	{SPECIES_CAMERUPT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ERUPTION},
	{SPECIES_TORKOAL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FURNACE},
	{SPECIES_SPOINK,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CONFUSE_RAY},
	{SPECIES_GRUMPIG,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TEETER_DANCE},
	{SPECIES_SPINDA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TEETER_DANCE},
	{SPECIES_TRAPINCH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SAND_TOMB},
	{SPECIES_VIBRAVA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BUG_BUZZ},
	{SPECIES_FLYGON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SANDSTORM},
	{SPECIES_CACNEA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PIN_MISSILE},
	{SPECIES_CACTURNE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_NEEDLE_ARM},
	{SPECIES_SWABLU,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROOST},
	{SPECIES_ALTARIA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ALLURING_VOICE},
	{SPECIES_ZANGOOSE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CRUSH_CLAW},
	{SPECIES_SEVIPER,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_TAIL},
	{SPECIES_LUNATONE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MOONLIGHT},
	{SPECIES_SOLROCK,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DAZZLING_GLEAM},
	{SPECIES_BARBOACH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MUD_SLAP},
	{SPECIES_WHISCASH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_EARTHQUAKE},
	{SPECIES_CORPHISH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CLAMP},
	{SPECIES_CRAWDAUNT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CIRCLE_THROW},
	{SPECIES_BALTOY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_RAPID_SPIN},
	{SPECIES_CLAYDOL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TWIN_BEAM},
	{SPECIES_LILEEP,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_INGRAIN},
	{SPECIES_CRADILY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_GIGA_DRAIN},
	{SPECIES_ANORITH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FURY_CUTTER},
	{SPECIES_ARMALDO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_X_SCISSOR},
	{SPECIES_FEEBAS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FLAIL},
	{SPECIES_MILOTIC,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CAPTIVATE},
	{SPECIES_CASTFORM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WEATHER_BALL},
	// DOES EACH CASTFORM NEED ITS OWN ENTRY??
	{SPECIES_KECLEON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CAMOUFLAGE},
	{SPECIES_SHUPPET,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SPITE},
	{SPECIES_BANETTE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_REVENGE},
	{SPECIES_DUSKULL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SHADOW_SNEAK},
	{SPECIES_DUSCLOPS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WILL_O_WISP},
	{SPECIES_TROPIUS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FRUIT_SNACK},
	{SPECIES_CHIMECHO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PSYCHIC_NOISE},
	{SPECIES_ABSOL,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_NIGHT_SLASH},
	{SPECIES_WYNAUT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SAFEGUARD},
	{SPECIES_SNORUNT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POWDER_SNOW},
	{SPECIES_GLALIE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FREEZE_DRY},
	{SPECIES_SPHEAL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ICE_BALL},
	{SPECIES_SEALEO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ENCORE},
	{SPECIES_WALREIN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ICE_FANG},
	{SPECIES_CLAMPERL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CLAMP},
	{SPECIES_HUNTAIL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SUCKER_PUNCH},
	{SPECIES_GOREBYSS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DRAINING_KISS},
	{SPECIES_RELICANTH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CURSE},
	{SPECIES_LUVDISC,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ATTRACT},
	{SPECIES_BAGON,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_HEADBUTT},
	{SPECIES_SHELGON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_IRON_DEFENSE},
	{SPECIES_SALAMENCE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FLY},
	{SPECIES_BELDUM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MAGNET_RISE},
	{SPECIES_METANG,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MAGNET_RISE},
	{SPECIES_METAGROSS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HEAVY_SLAM},
	{SPECIES_REGIROCK,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SHORE_UP},
	{SPECIES_REGICE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_GLACIATE},
	{SPECIES_REGISTEEL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ANCIENT_POWER},
	{SPECIES_LATIAS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LUSTER_PURGE},
	{SPECIES_LATIOS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CALM_MIND},
	{SPECIES_KYOGRE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ORIGIN_PULSE},
	{SPECIES_GROUDON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PRECIPICE_BLADES},
	{SPECIES_RAYQUAZA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DRAGON_ASCENT},
	{SPECIES_JIRACHI,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WISH},
	{SPECIES_DEOXYS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PSYCHO_BOOST},
	// ALL THE DEOXYS (DEOXI) ??
	{SPECIES_BULBASAUR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_GROWTH},
	{SPECIES_IVYSAUR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_VINE_WHIP},
	{SPECIES_VENUSAUR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SOLARBEAM},
	{SPECIES_CHARMANDER,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_SCRATCH},
	{SPECIES_CHARMELEON,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_FIRE_FANG},
	{SPECIES_CHARIZARD,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FIRE_BLAST},
	{SPECIES_SQUIRTLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TACKLE},
	{SPECIES_WARTORTLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_BUBBLEBEAM},
	{SPECIES_BLASTOISE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SHELL_CANNON},
	{SPECIES_CATERPIE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_STRING_SHOT},
	{SPECIES_METAPOD,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HARDEN},
	{SPECIES_BUTTERFREE,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_SILVER_WIND},
	{SPECIES_WEEDLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_STING},
	{SPECIES_KAKUNA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HARDEN},
	{SPECIES_BEEDRILL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TWINEEDLE},
	{SPECIES_PIDGEY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SAND_ATTACK},
	{SPECIES_PIDGEOTTO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WING_ATTACK},
	{SPECIES_PIDGEOT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AERIAL_ACE},
	{SPECIES_RATTATA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TAIL_WHIP},
	{SPECIES_RATICATE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SUPER_FANG},
	{SPECIES_SPEAROW,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PECK},
	{SPECIES_FEAROW,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FLY},
	{SPECIES_EKANS,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_WRAP},
	{SPECIES_ARBOK,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_GLARE},
	{SPECIES_PIKACHU,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_THUNDER_SHOCK},
	{SPECIES_RAICHU,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DISCHARGE},
	{SPECIES_SANDSHREW,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DEFENSE_CURL},
	{SPECIES_SANDSLASH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SPIKE_TACKLE},
	{SPECIES_NIDORAN_F,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_STING},
	{SPECIES_NIDORINA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FURY_SWIPES},
	{SPECIES_NIDOQUEEN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SUPERPOWER},
	{SPECIES_NIDORAN_M,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_STING},
	{SPECIES_NIDORINO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HORN_ATTACK},
	{SPECIES_NIDOKING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MEGAHORN},
	{SPECIES_CLEFAIRY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MOONLIGHT},
	{SPECIES_CLEFABLE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_COSMIC_POWER},
	{SPECIES_VULPIX,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WILL_O_WISP},
	{SPECIES_NINETALES,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MYSTICAL_FIRE},
	{SPECIES_JIGGLYPUFF,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_SING},
	{SPECIES_WIGGLYTUFF,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_ECHOED_VOICE},
	{SPECIES_ZUBAT,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_SUPERSONIC},
	{SPECIES_GOLBAT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LEECH_LIFE},
	{SPECIES_ODDISH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_INGRAIN},
	{SPECIES_GLOOM,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_ACID},
	{SPECIES_VILEPLUME,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POLLEN_CLOUD},
	{SPECIES_PARAS,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_ABSORB},
	{SPECIES_PARASECT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LEECH_LIFE},
	{SPECIES_VENONAT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_POWDER},
	{SPECIES_VENOMOTH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_EFFECT_SCALES},
	{SPECIES_DIGLETT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DIG},
	{SPECIES_DUGTRIO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DIG_TRIO},
	{SPECIES_MEOWTH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PAY_DAY},
	{SPECIES_PERSIAN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SLASH},
	{SPECIES_PSYDUCK,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CONFUSION},
	{SPECIES_GOLDUCK,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_EXTRASENSORY},
	{SPECIES_MANKEY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_RAGE},
	{SPECIES_PRIMEAPE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_THRASH},
	{SPECIES_GROWLITHE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HOWL},
	{SPECIES_ARCANINE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_EXTREMESPEED},
	{SPECIES_POLIWAG,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ENDURE},
	{SPECIES_POLIWHIRL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HYPNOSIS},
	{SPECIES_POLIWRATH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DYNAMICPUNCH},
	{SPECIES_ABRA,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_TELEPORT},
	{SPECIES_KADABRA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DISABLE},
	{SPECIES_ALAKAZAM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PSYSHOCK},
	{SPECIES_MACHOP,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROCK_THROW},
	{SPECIES_MACHOKE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_STRENGTH},
	{SPECIES_MACHAMP,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DYNAMICPUNCH},
	{SPECIES_BELLSPROUT,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_INGRAIN},
	{SPECIES_WEEPINBELL,			  ITEM_NONE,				 MOVE_NONE,				   MOVE_ACID},
	{SPECIES_VICTREEBEL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PITCHER_TRAP},
	{SPECIES_TENTACOOL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_STING},
	{SPECIES_TENTACRUEL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_PRISON},
	{SPECIES_GEODUDE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROCK_POLISH},
	{SPECIES_GRAVELER,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROLLOUT},
	{SPECIES_GOLEM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DYNAMITE},
	{SPECIES_PONYTA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_STOMP},
	{SPECIES_RAPIDASH,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FLARE_BLITZ},
	{SPECIES_SLOWPOKE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SLACK_OFF},
	{SPECIES_SLOWBRO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AMNESIA},
	{SPECIES_MAGNEMITE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_MAGNET_RISE},
	{SPECIES_MAGNETON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TRI_ATTACK},
	{SPECIES_FARFETCHD,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LEAF_BLADE},
	{SPECIES_DODUO,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_DOUBLE_HIT},
	{SPECIES_DODRIO,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DRILL_PECK},
	{SPECIES_SEEL,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_HORN_ATTACK},
	{SPECIES_DEWGONG,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CHILLING_WATER},
	{SPECIES_GRIMER,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SLUDGE},
	{SPECIES_MUK,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_GUNK_SHOT},
	{SPECIES_VOLTORB,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SELFDESTRUCT},
	{SPECIES_ELECTRODE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DISCHARGE},
	{SPECIES_KOFFING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POISON_GAS},
	{SPECIES_WEEZING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_COMPOUND_POISON},
	{SPECIES_RHYHORN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_TAKE_DOWN},
	{SPECIES_RHYDON,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HORN_DRILL},
	{SPECIES_HORSEA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SMOKESCREEN},
	{SPECIES_SEADRA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WHIRLPOOL},
	{SPECIES_GOLDEEN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HORN_ATTACK},
	{SPECIES_SEAKING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DRILL_RUN},
	{SPECIES_STARYU,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_RECOVER},
	{SPECIES_STARMIE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_COSMIC_POWER},
	{SPECIES_PINSIR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_VISE_GRIP},
	{SPECIES_MAGIKARP,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SPLASH},
	{SPECIES_GYARADOS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DRAGON_RAGE},
	{SPECIES_CROBAT,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LEECH_LIFE},
	{SPECIES_CHINCHOU,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_FLASH},
	{SPECIES_LANTURN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_LUMINESCENCE},
	{SPECIES_PICHU,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_NUZZLE},
	{SPECIES_IGGLYBUFF,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DISARMING_VOICE},
	{SPECIES_NATU,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_MIRACLE_EYE},
	{SPECIES_XATU,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_FUTURE_SIGHT},
	{SPECIES_BELLOSSOM,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SUN_DANCE},
	{SPECIES_MARILL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AQUA_TAIL},
	{SPECIES_AZUMARILL,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AQUA_TAIL},
	{SPECIES_POLITOED,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ECHOED_VOICE},
	{SPECIES_SLOWKING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CALM_MIND},
	{SPECIES_WOBBUFFET,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_DESTINY_BOND},
	{SPECIES_GIRAFARIG,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PSYCHIC_FANGS},
	{SPECIES_HERACROSS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_CIRCLE_THROW},
	{SPECIES_CORSOLA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AQUA_RING},
	{SPECIES_SKARMORY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_STEEL_WING},
	{SPECIES_KINGDRA,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_WHIRLPOOL},
	{SPECIES_PHANPY,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_POUND},
	{SPECIES_DONPHAN,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROLLOUT},
	{SPECIES_BUDEW,					  ITEM_NONE,				 MOVE_NONE,				   MOVE_SYNTHESIS},
	{SPECIES_ROSERADE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROSE_WHIP},
	{SPECIES_CHINGLING,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ECHOED_VOICE},
	{SPECIES_MAGNEZONE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ZAP_CANNON},
	{SPECIES_RHYPERIOR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_ROCK_BLAST},
	{SPECIES_GALLADE,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_PSYCHO_CUT},
	{SPECIES_PROBOPASS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_AERONASAL_ASSAULT},
	{SPECIES_DUSKNOIR,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_SPIRIT_SHACKLE},
	{SPECIES_FROSLASS,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_HEX},
	//{SPECIES_,				  ITEM_NONE,				 MOVE_NONE,				   MOVE_},
};

static const u8 sText_ResetStats[] = _("Reset Lowered Stats");
static const u8 sText_StatsPlus[] = _("+ All Stats");
static const u8 sText_StatsPlus2[] = _("++ All Stats");
static const u8 sText_CritHitsPlus[] = _("+ Critical Hit Chance");
static const u8 sText_FollowMe[] = _("Follow Me");
static const u8 sText_RecoverHP[] = _("Recover HP");
static const u8 sText_HealAllyHP[] = _("Heal Replacement HP");
static const u8 sText_PowerColon[] = _("Power: ");

// Functions
bool32 IsZMove(u32 move)
{
    return move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE;
}

bool32 CanUseZMove(u32 battler)
{
    u32 holdEffect = GetBattlerHoldEffect(battler, FALSE);

    // Check if Player has Z-Power Ring.
	/*  
    if (!TESTING && (battler == B_POSITION_PLAYER_LEFT
        || (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && battler == B_POSITION_PLAYER_RIGHT))
        && !CheckBagHasItem(ITEM_Z_POWER_RING, 1))
        return FALSE;
	*/
    // Add '| BATTLE_TYPE_FRONTIER' to below if issues occur
    if (gBattleTypeFlags & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_WALLY_TUTORIAL))
        return FALSE;

/*
    // Check if Trainer has already used a Z-Move.
    if (HasTrainerUsedGimmick(battler, GIMMICK_Z_MOVE))
        return FALSE;

    // Check if battler has another gimmick active.
    if (GetActiveGimmick(battler) != GIMMICK_NONE && GetActiveGimmick(battler) != GIMMICK_ULTRA_BURST)
        return FALSE;

    // Check if battler isn't holding a Z-Crystal.
    if (holdEffect != HOLD_EFFECT_Z_CRYSTAL)
        return FALSE;
*/
	
    // Check if battler is holding a Faded Ring.
    if (holdEffect == HOLD_EFFECT_FADED_RING)
        return FALSE;


    // All checks passed!
//    return TRUE;

	// Oops
	return FALSE;
}

u32 GetUsableZMove(u32 battler, u32 move)
{
 //   u32 item = gBattleMons[battler].item;
 //   u32 holdEffect = GetBattlerHoldEffect(battler, FALSE);
	u32 battlerSpecies = gBattleMons[battler].species;
 //   if (holdEffect == HOLD_EFFECT_Z_CRYSTAL)
   // {
//        u16 zMove = GetSignatureZMove(MOVE_NONE, gBattleMons[battler].species, ITEM_NONE);
		u32 zMove = gSpeciesInfo[battlerSpecies].innateMove;
        if (zMove != MOVE_NONE)
            return zMove;  // Signature z move exists

//       if (move != MOVE_NONE && zMove != MOVE_Z_STATUS && gMovesInfo[move].type == ItemId_GetSecondaryId(item))
//            return GetTypeBasedZMove(move);
   // }

    return MOVE_NONE;
}

void ActivateZMove(u32 battler)
{
    gBattleStruct->zmove.baseMoves[battler] = gBattleMons[battler].moves[gBattleStruct->chosenMovePositions[battler]];
    SetActiveGimmick(battler, GIMMICK_Z_MOVE);
}

bool32 IsViableZMove(u32 battler, u32 move)
{
    u32 item;
//    u32 holdEffect = GetBattlerHoldEffect(battler, FALSE);
//    int moveSlotIndex;

    item = gBattleMons[battler].item;

    if (gBattleStruct->gimmick.usableGimmick[battler] != GIMMICK_Z_MOVE)
        return FALSE;

 //   for (moveSlotIndex = 0; moveSlotIndex < MAX_LEARNED_MOVES; moveSlotIndex++)
//    {
//        if (gBattleMons[battler].moves[moveSlotIndex] == move && gBattleMons[battler].pp[moveSlotIndex] == 0)
//            return FALSE;
//    }

    // Check if Player has Z-Power Ring.
	/*
    if ((battler == B_POSITION_PLAYER_LEFT || (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && battler == B_POSITION_PLAYER_RIGHT))
        && !CheckBagHasItem(ITEM_Z_POWER_RING, 1))
    {
        return FALSE;
    }
*/
    // Check for signature Z-Move or type-based Z-Move.
   // if (holdEffect == HOLD_EFFECT_Z_CRYSTAL)
   // {
//        u16 zMove = GetSignatureZMove(move, gBattleMons[battler].species, item);
        u16 zMove = GetSignatureZMove(gBattleMons[battler].species);
        if (zMove != MOVE_NONE)
            return TRUE;

        if (move != MOVE_NONE && gMovesInfo[move].type == ItemId_GetSecondaryId(item))
            return TRUE;
    //}

    return FALSE;
}

void AssignUsableZMoves(u32 battler, u16 *moves)
{
    u32 i;
    gBattleStruct->zmove.possibleZMoves[battler] = 0;
    for (i = 0; i < MAX_LEARNED_MOVES; i++)
    {
        if (moves[i] != MOVE_NONE && IsViableZMove(battler, moves[i]))
            gBattleStruct->zmove.possibleZMoves[battler] |= 1u << i;
		
    }
}

bool32 TryChangeZTrigger(u32 battler, u32 moveIndex)
{
    bool32 viableZMove = (gBattleStruct->zmove.possibleZMoves[battler] & (1u << moveIndex)) != 0;

    if (gBattleStruct->zmove.viable && !viableZMove)
        HideGimmickTriggerSprite();   // Was a viable z move, now is not -> slide out
    else if (!gBattleStruct->zmove.viable && viableZMove)
        CreateGimmickTriggerSprite(battler);   // Was not a viable z move, now is -> slide back in

    gBattleStruct->zmove.viable = viableZMove;

    return viableZMove;
}

//u32 GetSignatureZMove(u32 move, u32 species, u32 item)
u32 GetSignatureZMove(u32 species)
{
    /*u32 i;

    // Check signature z move
    for (i = 0; i < ARRAY_COUNT(sSignatureZMoves); ++i)
    {
        if (sSignatureZMoves[i].item == ITEM_NONE && sSignatureZMoves[i].species == species &&  sSignatureZMoves[i].move == MOVE_NONE)
            return sSignatureZMoves[i].zmove;
    }
*/

	u32 zMove = gSpeciesInfo[species].innateMove;

    return zMove;
}

u32 GetTypeBasedZMove(u32 move)
{
    u32 moveType = gMovesInfo[move].type;

    if (moveType >= NUMBER_OF_MON_TYPES)
        moveType = TYPE_MYSTERY;

    // Z-Weather Ball changes types, however Revelation Dance, -ate ability affected moves, and Hidden Power do not
    if (gBattleStruct->dynamicMoveType && gMovesInfo[move].effect == EFFECT_WEATHER_BALL)
        moveType = gBattleStruct->dynamicMoveType & DYNAMIC_TYPE_MASK;

    // Get Z-Move from type
    if (gTypesInfo[moveType].zMove == MOVE_NONE) // failsafe
        return gTypesInfo[0].zMove;
    return gTypesInfo[moveType].zMove;
}

bool32 MoveSelectionDisplayZMove(u16 zmove, u32 battler)
{
    u32 i;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleResources->bufferA[battler][4]);
    u16 move = moveInfo->moves[gMoveSelectionCursor[battler]];

    PlaySE(SE_SELECT);
    gBattleStruct->zmove.viewing = TRUE;
	
    if (zmove != MOVE_NONE)
    {
        // Clear move slots
        for (i = 0; i < MAX_LEARNED_MOVES; ++i)
        {
            MoveSelectionDestroyCursorAt(i);
            StringCopy(gDisplayedStringBattle, gText_EmptyString2);
            BattlePutTextOnWindow(gDisplayedStringBattle, i + 3);
        }
/*
        if (IS_MOVE_STATUS(move))
        {
            u8 zEffect = gMovesInfo[move].zMove.effect;

            gDisplayedStringBattle[0] = EOS;

            if (zEffect == Z_EFFECT_CURSE)
            {
                if (moveInfo->monTypes[0] == TYPE_GHOST || moveInfo->monTypes[1] == TYPE_GHOST || moveInfo->monTypes[2] == TYPE_GHOST)
                    zEffect = Z_EFFECT_RECOVER_HP;
                else
                    zEffect = Z_EFFECT_ATK_UP_1;
            }

            switch (zEffect)
            {
            case Z_EFFECT_RESET_STATS:
                StringCopy(gDisplayedStringBattle, sText_ResetStats);
                break;
            case Z_EFFECT_ALL_STATS_UP_1:
                StringCopy(gDisplayedStringBattle, sText_StatsPlus);
                break;
            case Z_EFFECT_BOOST_CRITS:
                StringCopy(gDisplayedStringBattle, sText_CritHitsPlus);
                break;
            case Z_EFFECT_FOLLOW_ME:
                StringCopy(gDisplayedStringBattle, sText_FollowMe);
                break;
            case Z_EFFECT_RECOVER_HP:
                StringCopy(gDisplayedStringBattle, sText_RecoverHP);
                break;
            case Z_EFFECT_RESTORE_REPLACEMENT_HP:
                StringCopy(gDisplayedStringBattle, sText_HealAllyHP);
                break;
            case Z_EFFECT_ATK_UP_1:
            case Z_EFFECT_DEF_UP_1:
            case Z_EFFECT_SPD_UP_1:
            case Z_EFFECT_SPATK_UP_1:
            case Z_EFFECT_SPDEF_UP_1:
            case Z_EFFECT_ACC_UP_1:
            case Z_EFFECT_EVSN_UP_1:
                gDisplayedStringBattle[0] = CHAR_PLUS;
                gDisplayedStringBattle[1] = 0;
                gDisplayedStringBattle[2] = EOS;
                PREPARE_STAT_BUFFER(gBattleTextBuff1, zEffect - Z_EFFECT_ATK_UP_1 + 1);
                ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 2);
                break;
            case Z_EFFECT_ATK_UP_2:
            case Z_EFFECT_DEF_UP_2:
            case Z_EFFECT_SPD_UP_2:
            case Z_EFFECT_SPATK_UP_2:
            case Z_EFFECT_SPDEF_UP_2:
            case Z_EFFECT_ACC_UP_2:
            case Z_EFFECT_EVSN_UP_2:
                gDisplayedStringBattle[0] = CHAR_PLUS;
                gDisplayedStringBattle[1] = CHAR_PLUS;
                gDisplayedStringBattle[2] = 0;
                gDisplayedStringBattle[3] = EOS;
                PREPARE_STAT_BUFFER(gBattleTextBuff1, zEffect - Z_EFFECT_ATK_UP_2 + 1);
                ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 3);
                break;
            case Z_EFFECT_ATK_UP_3:
            case Z_EFFECT_DEF_UP_3:
            case Z_EFFECT_SPD_UP_3:
            case Z_EFFECT_SPATK_UP_3:
            case Z_EFFECT_SPDEF_UP_3:
            case Z_EFFECT_ACC_UP_3:
            case Z_EFFECT_EVSN_UP_3:
                gDisplayedStringBattle[0] = CHAR_PLUS;
                gDisplayedStringBattle[1] = CHAR_PLUS;
                gDisplayedStringBattle[2] = CHAR_PLUS;
                gDisplayedStringBattle[3] = 0;
                gDisplayedStringBattle[4] = EOS;
                PREPARE_STAT_BUFFER(gBattleTextBuff1, zEffect - Z_EFFECT_ATK_UP_3 + 1);
                ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gDisplayedStringBattle + 4);
                break;
            }

            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_NAME_3);
            gDisplayedStringBattle[0] = CHAR_Z;
            gDisplayedStringBattle[1] = CHAR_HYPHEN;
            StringCopy(gDisplayedStringBattle + 2, GetMoveName(move));
        }
        else*/ if (zmove == MOVE_EXTREME_EVOBOOST)
        {
            // Damaging move -> status z move
            StringCopy(gDisplayedStringBattle, sText_StatsPlus2);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_NAME_3);
            StringCopy(gDisplayedStringBattle, GetMoveName(zmove));
        }
        else
        {
            ZMoveSelectionDisplayPower(move, zmove);
            StringCopy(gDisplayedStringBattle, GetMoveName(zmove));
        }
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_NAME_1);

        ZMoveSelectionDisplayPpNumber(battler);
        ZMoveSelectionDisplayMoveType(zmove, battler);
        MoveSelectionCreateCursorAt(0, 0);
        return TRUE;
    }

    return FALSE;
}

static void ZMoveSelectionDisplayPower(u16 move, u16 zMove)
{
    u8 *txtPtr;
    u16 power = GetZMovePower(move);

  //  if (zMove >= MOVE_CATASTROPIKA)
        power = gMovesInfo[zMove].power;

    if (gMovesInfo[zMove].category != DAMAGE_CATEGORY_STATUS)
    {
        txtPtr = StringCopy(gDisplayedStringBattle, sText_PowerColon);
        ConvertIntToDecimalStringN(txtPtr, power, STR_CONV_MODE_LEFT_ALIGN, 3);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_NAME_3);
    }
}

static void ZMoveSelectionDisplayPpNumber(u32 battler)
{
    u8 *txtPtr;
	u8 currentPP = 1;
	u8 maxPP = 1;
	u32 i;
	
    if (gBattleResources->bufferA[battler][2] == TRUE) // Check if we didn't want to display pp number
        return;
	
    for (i = 0; i < MAX_SELECTABLE_MOVES; ++i)
	{
		if (gBattleMons[battler].moves[i] == gSpeciesInfo[gBattleMons[battler].species].innateMove)
		{
			currentPP = gBattleMons[battler].pp[i];
			maxPP = gMovesInfo[gBattleMons[battler].moves[i]].pp;
		}
	}
		
    SetPpNumbersPaletteInMoveSelection(battler);
//    txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, 1, STR_CONV_MODE_RIGHT_ALIGN, 2);
    txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, currentPP, STR_CONV_MODE_RIGHT_ALIGN, 2);
    *(txtPtr)++ = CHAR_SLASH;
 //   ConvertIntToDecimalStringN(txtPtr, 1, STR_CONV_MODE_RIGHT_ALIGN, 2);
    ConvertIntToDecimalStringN(txtPtr, maxPP, STR_CONV_MODE_RIGHT_ALIGN, 2);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_PP_REMAINING);
}

static void ZMoveSelectionDisplayMoveType(u16 zMove, u32 battler)
{
    u8 *txtPtr, *end;
    u32 zMoveType = GetMoveType(zMove);

    txtPtr = StringCopy(gDisplayedStringBattle, gText_MoveInterfaceType);
    *(txtPtr)++ = EXT_CTRL_CODE_BEGIN;
    *(txtPtr)++ = EXT_CTRL_CODE_FONT;
    *(txtPtr)++ = FONT_NORMAL;

    end = StringCopy(txtPtr, gTypesInfo[zMoveType].name);
    PrependFontIdToFit(txtPtr, end, FONT_NORMAL, WindowWidthPx(B_WIN_MOVE_TYPE) - 25);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_TYPE);
}

#define Z_EFFECT_BS_LENGTH  5
// This function kinda cheats by setting a return battle script to after the setzeffect various command
// and then jumping to a z effect script
void SetZEffect(void)
{
	// what if we just like ... not do any of this?
	gBattlescriptCurrInstr += Z_EFFECT_BS_LENGTH;
	
	/*
    u32 i;
    u32 effect = gMovesInfo[gBattleStruct->zmove.baseMoves[gBattlerAttacker]].zMove.effect;

    if (effect == Z_EFFECT_CURSE)
    {
        if (IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GHOST))
            effect = Z_EFFECT_RECOVER_HP;
        else
            effect = Z_EFFECT_ATK_UP_1;
    }

    gBattleScripting.savedStatChanger = gBattleScripting.statChanger;   // Save used move's stat changer (e.g. for Z-Growl)
    gBattleScripting.battler = gBattlerAttacker;

    switch (effect)
    {
    case Z_EFFECT_RESET_STATS:
        for (i = 0; i < NUM_BATTLE_STATS - 1; i++)
        {
            if (gBattleMons[gBattlerAttacker].statStages[i] < DEFAULT_STAT_STAGE)
                gBattleMons[gBattlerAttacker].statStages[i] = DEFAULT_STAT_STAGE;
        }
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_Z_RESET_STATS;
        BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
        gBattlescriptCurrInstr = BattleScript_ZEffectPrintString;
        break;
    case Z_EFFECT_ALL_STATS_UP_1:
    {
        bool32 canBoost = FALSE;
        for (i = STAT_ATK; i < NUM_STATS; i++) // Doesn't increase Acc or Evsn
        {
            if (STAT_STAGE(gBattlerAttacker, i) < 12)
            {
                canBoost = TRUE;
            }
        }
        if (canBoost)
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_Z_ALL_STATS_UP;
            BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
            gBattlescriptCurrInstr = BattleScript_AllStatsUpZMove;
        }
        else
        {
            gBattlescriptCurrInstr += Z_EFFECT_BS_LENGTH;
        }
        break;
    }
    case Z_EFFECT_BOOST_CRITS:
        if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY_ANY))
        {
            gBattleMons[gBattlerAttacker].status2 |= STATUS2_FOCUS_ENERGY;
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_Z_BOOST_CRITS;
            BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
            gBattlescriptCurrInstr = BattleScript_ZEffectPrintString;
        }
        else
        {
            gBattlescriptCurrInstr += Z_EFFECT_BS_LENGTH;
        }
        break;
    case Z_EFFECT_FOLLOW_ME:
        gSideTimers[GetBattlerSide(gBattlerAttacker)].followmeTimer = 1;
        gSideTimers[GetBattlerSide(gBattlerAttacker)].followmeTarget = gBattlerAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_Z_FOLLOW_ME;
        BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
        gBattlescriptCurrInstr = BattleScript_ZEffectPrintString;
        break;
    case Z_EFFECT_RECOVER_HP:
        if (gBattleMons[gBattlerAttacker].hp != gBattleMons[gBattlerAttacker].maxHP)
        {
            gBattleMoveDamage = (-1) * gBattleMons[gBattlerAttacker].maxHP;
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_Z_RECOVER_HP;
            BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
            gBattlescriptCurrInstr = BattleScript_RecoverHPZMove;
        }
        else
        {
            gBattlescriptCurrInstr += Z_EFFECT_BS_LENGTH;
        }
        break;
    case Z_EFFECT_RESTORE_REPLACEMENT_HP:
        gBattleStruct->zmove.healReplacement = TRUE;
        BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_Z_HP_TRAP;
        gBattlescriptCurrInstr = BattleScript_ZEffectPrintString;
        break;
    case Z_EFFECT_ATK_UP_1 ... Z_EFFECT_EVSN_UP_1:
        SET_STATCHANGER(effect - Z_EFFECT_ATK_UP_1 + 1, 1, FALSE);
        BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
        gBattlescriptCurrInstr = BattleScript_StatUpZMove;
        break;
    case Z_EFFECT_ATK_UP_2 ... Z_EFFECT_EVSN_UP_2:
        SET_STATCHANGER(effect - Z_EFFECT_ATK_UP_2 + 1, 2, FALSE);
        BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
        gBattlescriptCurrInstr = BattleScript_StatUpZMove;
        break;
    case Z_EFFECT_ATK_UP_3 ... Z_EFFECT_EVSN_UP_3:
        SET_STATCHANGER(effect - Z_EFFECT_ATK_UP_3 + 1, 3, FALSE);
        BattleScriptPush(gBattlescriptCurrInstr + Z_EFFECT_BS_LENGTH);
        gBattlescriptCurrInstr = BattleScript_StatUpZMove;
        break;
    default:
        gBattlescriptCurrInstr += Z_EFFECT_BS_LENGTH;
        break;
    }
	*/
}

u32 GetZMovePower(u32 species)
{
//	u32 move = MOVE_NONE;
//	u32 item = ITEM_NONE;
	
	u32 zMove = GetSignatureZMove(species);
	return gMovesInfo[zMove].power;
}
	
/*
u32 GetZMovePower(u32 move)
{
	
    if (gMovesInfo[move].category == DAMAGE_CATEGORY_STATUS)
        return 0;
	else
		return gMovesInfo[move].power;
	
    if (gMovesInfo[move].effect == EFFECT_OHKO)
        return 180;

    if (gMovesInfo[move].zMove.powerOverride > 0)
        return gMovesInfo[move].zMove.powerOverride;
    else
    {
        if (gMovesInfo[move].power >= 140)
            return 200;
        else if (gMovesInfo[move].power >= 130)
            return 195;
        else if (gMovesInfo[move].power >= 120)
            return 190;
        else if (gMovesInfo[move].power >= 110)
            return 185;
        else if (gMovesInfo[move].power >= 100)
            return 180;
        else if (gMovesInfo[move].power >= 90)
            return 175;
        else if (gMovesInfo[move].power >= 80)
            return 160;
        else if (gMovesInfo[move].power >= 70)
            return 140;
        else if (gMovesInfo[move].power >= 60)
            return 120;
        else
            return 100;
    }
	
}
*/

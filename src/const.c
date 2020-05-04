/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,	"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,	"wand"		},
    {   ITEM_STAFF,	"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_NULL6,     "null6"         },
    {   ITEM_NULL7,     "null7"         },
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,	"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,	"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,	"key"		},
    {	ITEM_FOOD,	"food"		},
    {	ITEM_MONEY,	"money"		},
    {	ITEM_BOAT,	"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,	"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,	"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,	"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {   ITEM_JUKEBOX,	"jukebox"	},
    {  ITEM_SKELETON,	"skeleton"},
    {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",	OBJ_VNUM_SCHOOL_SWORD,	WEAPON_SWORD,	&gsn_sword	},
   { "mace",	OBJ_VNUM_SCHOOL_MACE,	WEAPON_MACE,	&gsn_mace 	},
   { "dagger",	OBJ_VNUM_SCHOOL_DAGGER,	WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",	OBJ_VNUM_SCHOOL_AXE,	WEAPON_AXE,	&gsn_axe	},
   { "spear",	OBJ_VNUM_SCHOOL_STAFF,	WEAPON_SPEAR,	&gsn_spear	},
   { "flail",	OBJ_VNUM_SCHOOL_FLAIL,	WEAPON_FLAIL,	&gsn_flail	},
   { "whip",	OBJ_VNUM_SCHOOL_WHIP,	WEAPON_WHIP,	&gsn_whip	},
   { "polearm",	OBJ_VNUM_SCHOOL_POLEARM,WEAPON_POLEARM,	&gsn_polearm	},
    { "staff", OBJ_VNUM_SCHOOL_POLEARM, WEAPON_STAFF, &gsn_staff	},
   { NULL,	0,				0,	NULL		}
};


 
/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           WIZ_ON,         IM },
   {    "prefix",	WIZ_PREFIX,	IM },
   {    "ticks",        WIZ_TICKS,      IM },
   {    "logins",       WIZ_LOGINS,     IM },
   {    "sites",        WIZ_SITES,      L4 },
   {    "links",        WIZ_LINKS,      L7 },
   {	"newbies",	WIZ_NEWBIE,	IM },
   {	"spam",		WIZ_SPAM,	L5 },
   {    "deaths",       WIZ_DEATHS,     IM },
   {    "resets",       WIZ_RESETS,     L4 },
   {    "mobdeaths",    WIZ_MOBDEATHS,  L4 },
   {    "flags",	WIZ_FLAGS,	L5 },
   {	"penalties",	WIZ_PENALTIES,	L5 },
   {	"saccing",	WIZ_SACCING,	L5 },
   {	"levels",	WIZ_LEVELS,	IM },
   {	"load",		WIZ_LOAD,	L2 },
   {	"restore",	WIZ_RESTORE,	L2 },
   {	"snoops",	WIZ_SNOOPS,	L2 },
   {	"switches",	WIZ_SWITCHES,	L2 },
   {	"secure",	WIZ_SECURE,	L1 },
   {	NULL,		0,		0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[MAX_DAMAGE_MESSAGE]	=
{
    { 	"none",		"hit",		-1		},  /*  0 */
    {	"slice",	"slice", 	DAM_SLASH	},	
    {   "stab",		"stab",		DAM_PIERCE	},
    {	"slash",	"slash",	DAM_SLASH	},
    {	"whip",		"whip",		DAM_SLASH	},
    {   "claw",		"claw",		DAM_SLASH	},  /*  5 */
    {	"blast",	"blast",	DAM_BASH	},
    {   "pound",	"pound",	DAM_BASH	},
    {	"crush",	"crush",	DAM_BASH	},
    {   "grep",		"grep",		DAM_SLASH	},
    {	"bite",		"bite",		DAM_PIERCE	},  /* 10 */
    {   "pierce",	"pierce",	DAM_PIERCE	},
    {   "suction",	"suction",	DAM_BASH	},
    {	"beating",	"beating",	DAM_BASH	},
    {   "digestion",	"digestion",	DAM_ACID	},
    {	"charge",	"charge",	DAM_BASH	},  /* 15 */
    { 	"slap",		"slap",		DAM_BASH	},
    {	"punch",	"punch",	DAM_BASH	},
    {	"wrath",	"wrath",	DAM_ENERGY	},
    {	"magic",	"magic",	DAM_ENERGY	},
    {   "divine",	"divine power",	DAM_HOLY	},  /* 20 */
    {	"cleave",	"cleave",	DAM_SLASH	},
    {	"scratch",	"scratch",	DAM_PIERCE	},
    {   "peck",		"peck",		DAM_PIERCE	},
    {   "peckb",	"peck",		DAM_BASH	},
    {   "chop",		"chop",		DAM_SLASH	},  /* 25 */
    {   "sting",	"sting",	DAM_PIERCE	},
    {   "smash",	 "smash",	DAM_BASH	},
    {   "shbite",	"shocking bite",DAM_LIGHTNING	},
    {	"flbite",	"flaming bite", DAM_FIRE	},
    {	"frbite",	"freezing bite", DAM_COLD	},  /* 30 */
    {	"acbite",	"acidic bite", 	DAM_ACID	},
    {	"chomp",	"chomp",	DAM_PIERCE	},
    {  	"drain",	"life drain",	DAM_NEGATIVE	},
    {   "thrust",	"thrust",	DAM_PIERCE	},
    {   "slime",	"slime",	DAM_ACID	}, /* 35 */
    {	"shock",	"shock",	DAM_LIGHTNING	},
    {   "thwack",	"thwack",	DAM_BASH	},
    {   "flame",	"flame",	DAM_FIRE	},
    {   "chill",	"chill",	DAM_COLD	},
    { "light",	"blast of light", 	DAM_ENERGY	}, /* 40 */
    { "nature", "force of nature", DAM_ENERGY },
    {   NULL,		NULL,		0		}
};

/* race table */
const 	struct	race_type	race_table	[]		=
{
/*
    {
	name,		pc_race?,
	act bits,	aff_by bits,	off bits,
	imm,		res,		vuln,
	form,		parts 
    },
*/
    { "unique",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0 },

    { 
	"human",		TRUE, 
	0,		0, 		0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"dwarf",		TRUE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"elf",			TRUE,
        0,              AFF_SNEAK|AFF_INFRARED,   0,
	0,		RES_CHARM|RES_FIRE,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "grey-elf",             TRUE,
        0,              AFF_SNEAK|AFF_INFRARED,    0,
        0,              RES_CHARM,      VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "dark-elf",             TRUE,
        0,              AFF_SNEAK|AFF_INFRARED,    0,
        0,              RES_CHARM|RES_COLD,      VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "centaur",                  TRUE,
	0,		0,		0,
        0,              RES_CHARM|RES_BASH,      VULN_MENTAL,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "troll",                TRUE,
	0,		0,		0,
        0,              RES_POISON|RES_DISEASE|RES_WEAPON,             VULN_ACID|VULN_FIRE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"giant",		TRUE,
	0,		0,		0,
	0,		RES_WEAPON|RES_BASH,	VULN_MENTAL,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "gnome",                TRUE,
	0,		0,		0,
        0,              RES_MENTAL|RES_CHARM,              VULN_BASH,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "draconian",                TRUE,
        0,              AFF_FLYING,   0,
        IMM_POISON,              RES_SLASH|RES_ACID|RES_DISEASE,   VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "ethereal",                TRUE,
	0,		AFF_PASS_DOOR|AFF_FLYING|AFF_INFRARED,	0,
	IMM_DISEASE|IMM_POISON,		RES_WEAPON|RES_MENTAL|RES_BASH, VULN_DROWNING|VULN_LIGHTNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"changeling", TRUE,
	0,	0,	0,
	0,	0,	0,
	H|M|V,	A|B|V|D|E|F|G|H|I|J|K
    },
    {

	"bat", 			FALSE,
	0,		AFF_FLYING|AFF_DARK_VISION,	OFF_DODGE|OFF_FAST,
	0,		0,		VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P
    },

    {
	"bear",			FALSE,
	0,		0,		OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {
	"cat",			FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {
	"centipede",		FALSE,
	0,		AFF_DARK_VISION,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K	
    },

    {
	"duergar",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING|VULN_LIGHT,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"dog",			FALSE,
	0,		0,		OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
    },

    {
	"doll",			FALSE,
	0,		0,		0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K
    },

    { 	"dragon", 		FALSE, 
	0, 			AFF_INFRARED|AFF_FLYING,	0,
	0,			RES_FIRE|RES_BASH|RES_CHARM, 
	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },

    {
	"fido",			FALSE,
	0,		0,		OFF_DODGE|ASSIST_RACE,
	0,		0,			VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },		

    {
	"fish",	FALSE,	
	0,	AFF_SWIM,	OFF_DODGE,
	0,	RES_SUMMON|RES_CHARM,	0,
	A|G|V,	A|D|F|K|O|Q|X
    },   

    {
	"fox",			FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    {
	"goblin",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"hobgoblin",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y
    },

    {
	"kobold",		FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {
	"lizard",		FALSE,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {
	"modron",		FALSE,
	0,		AFF_INFRARED,		ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K
    },

    {
	"pig",			FALSE,
	0,		0,		0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },	

    {
	"rabbit",		FALSE,
	0,		0,		OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K
    },
    
    {
	"school monster",	FALSE,
	ACT_NOALIGN,		0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },	

    {
	"snake",		FALSE,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },
 
    {
	"song bird",		FALSE,
	0,		AFF_FLYING,		OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {
	"water fowl",		FALSE,
	0,		AFF_SWIM|AFF_FLYING,	0,
	0,		RES_DROWNING,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },		
  
    {
	"wolf",			FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,	
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {
	"wyvern",		FALSE,
	0,		AFF_FLYING|AFF_DETECT_INVIS|AFF_DETECT_HIDDEN,
	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,		A|C|D|E|F|H|J|K|Q|V|X
    },

    {
	"unique",		FALSE,
	0,		0,		0,
	0,		0,		0,		
	0,		0
    },


    {
	NULL, 0, 0, 0, 0, 0, 0
    }
};

const	struct	pc_race_type	pc_race_table	[]	=
{
    { "null race", "", 0, 0, { "" }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 13, 13, 13, 13, 13 }, { 18, 18, 18, 18, 18 }, 0 },
 
/*
    {
        "race name",    short name,     align, xpadd,
	{ bonus skills },
	{ stat rolling modifier },	{ max stats }, size 
    },
*/
    {
        "human",        "Human",        0, 0,
	{ "" }, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 0 }, { 20, 20, 20, 20, 20 }, SIZE_MEDIUM
    },

    {
        "dwarf",        "Dwarf",        0, 250,
        { "berserk" }, { 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 },
        { 2, -2, 1, -1, 3 }, { 22, 18, 21, 16, 25 }, SIZE_MEDIUM
    },

    { 	
        "elf",          " Elf ",        3, 400, 
        { "awareness","forest blending" }, { 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 },
        { -2, 3, 0, 2, -2 }, { 18, 25, 20, 23, 17 }, SIZE_MEDIUM
    },

    { 	
        "grey-elf",          "G-Elf",        4, 400, 
        { "awareness","forest blending" }, { 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1 },
        { -1, 2, 2, 2, -2 }, { 17, 24, 24, 22, 16 }, SIZE_MEDIUM
    },

    { 	
        "dark-elf",     "D-Elf",        5, 400,
        { "globe of darkness" }, 
	{ 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1 },
        { -1, 3, -2, 2, -1 }, { 18, 24, 20, 24, 17 }, SIZE_MEDIUM
    },

    { 	
        "centaur",          " Cen ",        3, 500, 
        { "rear kick", "enhanced damage", "kick", "door bash" }, 
	{ 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1 },
        { 2, -2, -2, 0, 2 }, { 24, 17, 18, 20, 22 }, SIZE_LARGE
    },

    { 	
        "troll",        "Troll",        5, 450, 
        { "regeneration","enhanced damage", "bash", "door bash" }, 
{ 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
        { 3, -3, -2, -2, 3 }, { 25, 16, 17, 19, 24 }, SIZE_LARGE
    },

    {
        "giant",        "Giant",        4, 500, 
        { "door bash","enhanced damage", "bash" }, 
	{ 1, 0, 1, 0, 0, 1, 0,0, 0, 0, 0},
        { 3, -2, -2, -1, 3 }, { 25, 16, 17, 18, 24 }, SIZE_LARGE
    },

    { 	
        "gnome",        "Gnome",        0, 200, 
        { "staves", "wands" }, 
	{ 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1 },
        { -2, 3, 2, 0, -1 }, { 18, 23, 25, 20, 19 }, SIZE_SMALL
    },

    { 	
        "draconian",    "Draco",        5, 300,
        { "breath fire" }, { 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0 },
        { 2, 1, -1, 0, 0 }, { 23, 23, 20, 18, 20 }, SIZE_MEDIUM
    },

    { 	
        "ethereal",      " Eth ",        1, 400, 
        { "gaseous form" }, { 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1 },
        { -3, 2, 2, 2, -1 }, { 17, 23, 22, 23, 18 }, SIZE_SMALL
    },

    {
	"changeling",	"Chnge",	0, 500,
	{"shapeshift", "trinal wield"},	{1,1,0,1,1,0,0,1,0,0,1},
	{-2,3,2,3,-3},	{17,24,22,25,15},	SIZE_MEDIUM
    },

};

	
      	

/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
    {
        "warrior", "War",  0, 0, OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 9633, 8818 },  75,  20,  -10,  8, 19, FALSE,
	"class basics", "class default"
    },

    {
        "thief", "Thi",  0, 0, OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 9639, 8850 },  75,  20,  -4,  8, 17, FALSE,
	"class basics", "class default"
    },

    {
        "cleric", "Cle",  0, 0, OBJ_VNUM_SCHOOL_MACE,
        { 3003, 9619, 8836 },  75,  20, 2,  7, 15, TRUE,
	"class  basics", "class default"
    },

    {
        "paladin", "Pal",  3, 500, OBJ_VNUM_SCHOOL_SWORD,
        { 9753, 8835, 9754 },  75,  20, -6,  8,  18, TRUE,
        "class basics", "class default"
    },

    {
        "anti-paladin", "A-P",  5, 400, OBJ_VNUM_SCHOOL_SWORD,
        { 8820, 9780, 9751 },  75,  20, -6,  8,  18, TRUE,
        "class basics", "class default"
    },

    {
        "ranger", "Ran",  0, 250, OBJ_VNUM_SCHOOL_SWORD,
        { 6156, 1874, 1890, 1891 },  75,  20, -8,  8,  18, FALSE,
        "class basics", "class default"
    },

    {
        "monk", "Mon",  1, 300, OBJ_VNUM_SCHOOL_DAGGER,
        { 9597, 8819 },  75,  20, 2,  7,  15, FALSE,
        "class basics", "class default"
    },

    {
        "channeler", "Cha",  0, 100, OBJ_VNUM_SCHOOL_DAGGER,
        { 9617, 9618, 9648, 8817 },  75,  20, 2,  7,  15, TRUE,
        "class basics", "class default"
    },

    {
        "assassin", "Asn",  5, 400, OBJ_VNUM_SCHOOL_SWORD,
        { 8837, 9776, 9777 },  75,  20, -4,  8,  17, FALSE,
        "class basics", "class default"
    },

    {
        "necromancer", "Nec",  5, 100, OBJ_VNUM_SCHOOL_DAGGER,
        { 8815, 9727, 9728 },  75,  20, -2,  7,  15, TRUE,
        "class basics", "class default"
    },

    {
        "elementalist", "Ele",  0, 200, OBJ_VNUM_SCHOOL_DAGGER,
        { 8897, 9779 },  75,  20, -4,  8,  16, TRUE,
        "class basics", "class default"
    },

};



/*
 * Titles.
 */
char *	const			title_table	[MAX_CLASS][MAX_LEVEL+1][2] =
{
    {
	{ "Man",			"Woman"				},

	{ "Swordpupil",			"Swordpupil"			},
	{ "Recruit",			"Recruit"			},
	{ "Sentry",			"Sentress"			},
	{ "Fighter",			"Fighter"			},
	{ "Soldier",			"Soldier"			},

	{ "Warrior",			"Warrior"			},
	{ "Veteran",			"Veteran"			},
	{ "Swordsman",			"Swordswoman"			},
	{ "Fencer",			"Fenceress"			},
	{ "Combatant",			"Combatess"			},

	{ "Hero",			"Heroine"			},
	{ "Myrmidon",			"Myrmidon"			},
	{ "Swashbuckler",		"Swashbuckleress"		},
	{ "Mercenary",			"Mercenaress"			},
	{ "Swordmaster",		"Swordmistress"			},

	{ "Lieutenant",			"Lieutenant"			},
	{ "Champion",			"Lady Champion"			},
	{ "Dragoon",			"Lady Dragoon"			},
	{ "Cavalier",			"Lady Cavalier"			},
	{ "Knight",			"Lady Knight"			},

	{ "Grand Knight",		"Grand Knight"			},
	{ "Master Knight",		"Master Knight"			},
        { "Grand Warrior",              "Grand Warrior"                       },
        { "Greater Combatant",           "Greater Combatant"                 },
	{ "Demon Slayer",		"Demon Slayer"			},

	{ "Greater Demon Slayer",	"Greater Demon Slayer"		},
	{ "Dragon Slayer",		"Dragon Slayer"			},
	{ "Greater Dragon Slayer",	"Greater Dragon Slayer"		},
	{ "Underlord",			"Underlord"			},
	{ "Overlord",			"Overlord"			},

	{ "Baron of Thunder",		"Baroness of Thunder"		},
	{ "Baron of Storms",		"Baroness of Storms"		},
	{ "Baron of Tornadoes",		"Baroness of Tornadoes"		},
	{ "Baron of Hurricanes",	"Baroness of Hurricanes"	},
	{ "Baron of Meteors",		"Baroness of Meteors"		},

        { "Rock Crusher",       "Rock Crusher"  },
        { "Shield Cleaver",     "Shield Cleaver"        },
        { "Student of War",     "Student of War"        },
        { "Learned of War",     "Learned of War"        },
        { "Master of War",      "Mistress of War"       },

        { "Bone Shatterer",  "Bone Shatterer"     },
        { "Skull Cracker",  "Skull Cracker"     },
        { "Armor Splitter",   "Armor Splitter"    },
        { "Sword Breaker",      "Sword Breaker"         },
        { "Weapon Master",      "Weapon Mistress"       },
       
        { "Student of Battle",  "Student of Battle"     },
        { "Learned of Battle",  "Learned of Battle"     },
        { "Master of Battle",   "Mistress of Battle"    },
        { "Grand Battle Master",        "Grand Battle Mistress"   },
        { "Supreme Battle Master",      "Supreme Battle Mistress"       },

	{ "Knight Hero",		"Knight Heroine"		},
        { "Avatar",              "Avatar"                 },
        { "Angel",               "Angel"                  },
        { "Demigod",             "Demigoddess"            },
        { "Immortal",           "Immortal"              },
        { "God",                 "God"                    },
        { "Deity",               "Deity"                  },
        { "Supreme Master",      "Supreme Mistress"       },
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
    {
	{ "Man",			"Woman"				},

	{ "Pilferer",			"Pilferess"			},
	{ "Footpad",			"Footpad"			},
	{ "Filcher",			"Filcheress"			},
	{ "Pick-Pocket",		"Pick-Pocket"			},
	{ "Sneak",			"Sneak"				},

	{ "Pincher",			"Pincheress"			},
	{ "Cut-Purse",			"Cut-Purse"			},
	{ "Snatcher",			"Snatcheress"			},
	{ "Sharper",			"Sharpress"			},
	{ "Rogue",			"Rogue"				},

	{ "Robber",			"Robber"			},
	{ "Magsman",			"Magswoman"			},
	{ "Highwayman",			"Highwaywoman"			},
	{ "Burglar",			"Burglaress"			},
	{ "Thief",			"Thief"				},

	{ "Knifer",			"Knifer"			},
	{ "Quick-Blade",		"Quick-Blade"			},
	{ "Killer",			"Murderess"			},
	{ "Brigand",			"Brigand"			},
	{ "Cut-Throat",			"Cut-Throat"			},

	{ "Spy",			"Spy"				},
	{ "Grand Spy",			"Grand Spy"			},
	{ "Master Spy",			"Master Spy"			},
        { "Greater Thief",              "Greater Thief"                      },
        { "Master of Thievery",         "Mistress of Thievery"              },

	{ "Master of Vision",		"Mistress of Vision"		},
	{ "Master of Hearing",		"Mistress of Hearing"		},
	{ "Master of Smell",		"Mistress of Smell"		},
	{ "Master of Taste",		"Mistress of Taste"		},
	{ "Master of Touch",		"Mistress of Touch"		},

	{ "Crime Lord",			"Crime Mistress"		},
	{ "Infamous Crime Lord",	"Infamous Crime Mistress"	},
	{ "Greater Crime Lord",		"Greater Crime Mistress"	},
	{ "Master Crime Lord",		"Master Crime Mistress"		},
	{ "Godfather",			"Godmother"			},

        { "Student of Speed",   "Student of Speed"      },
        { "Learned of Speed",   "Learned of Speed"      },
        { "Master of Speed",    "Mistress of Speed"     },
        { "Shadow",     "Shadow"        },
        { "Master of Shadows",  "Mistress of Shadows"   },

        { "Hidden",     "Hidden"        },
        { "Unseen",     "Unseen"        },
        { "Undetectable",       "Undetectable"  },
        { "Untouchable",        "Untouchable"   },
        { "Covert",    "Covert"       },

        { "Master of Disguise",     "Mistress of Disguise"        },
        { "Master Blackjacker",    "Mistress Blackjacker"       },
        { "Master Backstabber",     "Mistress Backstabber"        },
        { "Master of the Blade",   "Mistress of the Blade"      },
        { "Grand Master of Thieves",    "Grand Mistress of Thieves"     },

        { "Thief Hero",              "Thief Heroine"              },
        { "Avatar",            "Avatar",              },
        { "Angel",             "Angel"                },
        { "Demigod",       "Demigoddess"      },
        { "Immortal",           "Immortal"             },
        { "God",           "God",             },
        { "Deity",         "Deity"            },
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    },
    {
	{ "Man",			"Woman"				},

	{ "Apprentice of Magic",	"Apprentice of Magic"		},
        { "Altar Boy",  "Altar Girl"    },
        { "Believer",   "Believer"      },
        { "Attendant", "Attendant"      },
        { "Acolyte", "Acolyte"  },

        { "Novice", "Novice"    },
        { "Hermit", "Hermit"    },
        { "Monk", "Nun"         },
        { "Adept", "Adept"      },
        { "Deacon", "Deaconess" },

        { "Seminary Student", "Seminary Student"        },
        { "Seminary Scholar", "Seminary Scholar"        },
        { "Receiver of the Call", "Receiver of the Call" },
        { "Chosen", "Chosen"    },
        { "Ordained", "Ordained"        },

        { "Priest", "Priestess"         },
        { "Minister", "Lady Minister"   },
        { "Curate", "Curate"    },
        { "Canon", "Canon"      },
        { "Healer", "Healeress"         },

        { "Chaplain", "Chaplain"        },
        { "Expositor", "Expositoress"   },
        { "Missionary", "Missionary"    },
        { "Seminary Teacher", "Seminary Teacher"        },
        { "Bishop", "Bishop"    },

        { "Arch Bishop", "Arch Bishop"  },
        { "Patriarch", "Matriarch"      },
        { "Elder Patriarch", "Elder Matriarch"  },
        { "Grand Patriarch", "Grand Matriarch"  },
        { "Great Patriarch", "Great Matriarch"  },

        { "Exorcist", "Exorcist"        },
        { "Demon Killer", "Demon Killer"        },
        { "Greater Demon Killer", "Greater Demon Killer"        },
        { "Cardinal of the Earth", "Cardinal of the Earth"      },
        { "Cardinal of the Seas", "Cardinal of the Seas"        },

        { "Cardinal of the Winds", "Cardinal of the Winds"      },
        { "Cardinal of the Fire", "Cardinal of the Fire"        },
        { "Cardinal of the Ether", "Cardinal of the Ether"      },
        { "Cardinal of the North", "Cardinal of the North"      },
        { "Cardinal of the East", "Cardinal of the East"        },

        { "Cardinal of the South", "Cardinal of the South"      },
        { "Cardinal of the West", "Cardinal of the West"        },
        { "Avatar of an Immortal", "Avatar of an Immortal"      },
        { "Avatar of a Deity", "Avatar of a Deity"      },
        { "Avatar of a God", "Avatar of a God"  },

        { "Avatar of an Implementor", "Avatar of an Implementor"        },
        { "Master of Divinity", "Mistress of Divinity"  },
        { "Saint", "Saint"      },
        { "Divine", "Divine"    },
        { "Holy Father", "Holy Mother"  },

        { "Holy Hero",                  "Holy Heroine"                  },
        { "Avatar",            "Avatar"               },
        { "Angel",             "Angel"                },
        { "Demigod",           "Demigoddess"          },
        { "Immortal",          "Immortal"             },
        { "God",               "Goddess"              },
        { "Deity",             "Deity"                },
        { "Supremity",         "Supremity"            },
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

        { "Paladin Pupil", "Paladin Pupil" },
        { "Scullery Man", "Scullery Maid" },
        { "Squire Candidate", "Squire Candidate" },
        { "Shield Bearer", "Shield Bearer" },
        { "Sword Bearer", "Sword Bearer" },

        { "Bow Bearer", "Bow Bearer" },
        { "Standard Bearer", "Standard Bearer" },
        { "Horseman", "Horsewoman" },
        { "Squire Initiate", "Squire Initiate" },
        { "Squire", "Squire" },

        { "Footman", "Footwoman" },
        { "Pikeman", "Pikewoman" },
        { "Bowman", "Bowwoman" },
        { "Swordsman", "Swordsman" },
        { "Honorable", "Honorable" },

        { "Noble", "Noble" },
        { "Trustworthy", "Trustworthy" },
        { "Truthful", "Truthful" },
        { "Chivalrous", "Chivalrous" },
        { "Paladin", "Paladin" },

        { "Questor", "Questor" },
        { "Cavalier", "Cavalier" },
        { "Champion", "Champion" },
        { "Knight of Renown", "Knight of Renown" },
        { "Paladin Knight", "Paladin Knight" },

        { "Templar Initiate", "Templar Initiate" },
        { "Priest-Knight", "Priestess-Knight" },
        { "Knight of the Cross", "Knight of the Cross" },
        { "Champion of the Cross", "Champion of the Cross" },
        { "Knight Templar", "Lady Templar" },

        { "Warrior of the White Rose", "Warrior of the White Rose" },
        { "Knight of the White Rose", "Lady of the White Rose" },
        { "Master of the White Rose", "Mistress of the White Rose" },
        { "Prince of the White Rose", "Princess of the White Rose" },
        { "King of the White Rose", "Queen of the White Rose" },

        { "Valiant", "Valiant" },
        { "Courageous", "Courageous" },
        { "Healer-Knight", "Healer-Lady" },
        { "Avenger", "Avenger" },
        { "Defender", "Defender" },

        { "Protector of Innocents", "Protector of Innocents" },
        { "Champion of Innocents", "Champion of Innocents" },
        { "Champion of the Templars", "Champion of the Templars" },
        { "Priest of the Templars", "Priestess of the Templars" },
        { "High Priest of the Templars", "High Priestess of the Templars" },

        { "Lord of the Templars", "Lady of the Templars" },
        { "Hammer of Heretics", "Hammer of Heretics" },
        { "Slayer of Infidels", "Slayer of Infidels" },
        { "Pious", "Pious" },
        { "Holy Knight", "Holy Lady" },

        { "Paladin Hero",                  "Paladin Heroine"                  },
        { "Avatar",            "Avatar"               },
        { "Angel",             "Angel"                },
        { "Demigod",           "Demigoddess"          },
        { "Immortal",          "Immortal"             },
        { "God",               "Goddess"              },
        { "Deity",             "Deity"                },
        { "Supremity",         "Supremity"            },
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

        { "Scum", "Scum" },
        { "Bully", "Bully" },
        { "Thug", "Moll" },
        { "Brute", "Brute" },
        { "Ruffian", "Ruffian" },

        { "Pillager", "Pillager" },
        { "Destroyer", "Destroyer" },
        { "Arsonist", "Arsonist" },
        { "Hired Killer", "Hired Killer" },
        { "Brigand", "Brigand" },

        { "Mercenary", "Mercenary" },
        { "Black Sword", "Black Sword" },
        { "Crimson Sword", "Crimson Sword" },
        { "Black Hearted", "Black Hearted" },
        { "Cruel", "Cruel" },

        { "Stealer", "Stealer" },
        { "Infamous", "Infamous" },
        { "Despised", "Despised" },
        { "Complete Bastard", "Complete Bitch" },
        { "Anti-Paladin", "Anti-Paladin" },

        { "Evil Fighter", "Evil Fighter" },
        { "Rogue Knight", "Rogue Lady" },
        { "Evil Champion", "Evil Champion" },
        { "Slayer of Innocents", "Slayer of Innocents" },
        { "Black Knight", "Black Lady" },

        { "Crimson Knight", "Crimson Lady" },
        { "Knight of Brimstone", "Lady of Brimstone" },
        { "Knight of the Inverted Cross", "Lady of the Inverted Cross" },
        { "Knight of Pain", "Lady of Pain" },
        { "Knight of Darkness", "Lady of Darkness" },

        { "Footman of Darkness", "Footwoman of Darkness" },
        { "Cavalier of Darkness", "Cavalier of Darkness" },
        { "Captain of Darkness", "Captain of Darkness" },
        { "General of Darkness", "General of Darkness" },
        { "Field Marshall of Darkness", "Field Marshall of Darkness" },

        { "Knight of the Apocalypse", "Lady of the Apocalypse" },
        { "LightSlayer", "LightSlayer" },
        { "Invoker of Suffering", "Invoker of Suffering" },
        { "Arch-Fiend", "Arch-Friend" },
        { "Evil Lord", "Evil Lady" },

        { "Evil Prince", "Evil Princess" },
        { "Evil King", "Evil Queen" },
        { "Destroyer of Hope", "Destroyer of Hope" },
        { "Spreader of Pestilence", "Spreader of Pestilence" },
        { "Bringer of Famine", "Bringer of Famine" },

        { "Harbinger of Death", "Harbinger of Death" },
        { "Herald of Doom", "Herald of Doom" },
        { "Dark Slayer", "Dark Slayer" },
        { "Evil Incarnate", "Evil Incarnate" },
        { "Hand of the Dark", "Hand of the Dark" },

        { "Anti-Hero",                  "Anti-Heroine"                  },
        { "Avatar",            "Avatar"               },
        { "Angel",             "Angel"                },
        { "Demigod",           "Demigoddess"          },
        { "Immortal",          "Immortal"             },
        { "God",               "Goddess"              },
        { "Deity",             "Deity"                },
        { "Supremity",         "Supremity"            },
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

        { "Forest Pupil", "Forest Pupil" },
        { "Forest Recruit", "Forest Recruit" },
        { "Forest Squire", "Forest Squire" },
        { "Forester", "Forester" },
        { "Apprentice Forester", "Apprentice Forester" },

        { "Forester", "Forester" },
        { "Master Forester", "Mistress Forester" },
        { "Trapper", "Trapper" },
        { "Hunter", "Hunter" },
        { "Scout", "Scout" },

        { "Master Scout", "Mistress Scout" },
        { "Tracker", "Tracker" },
        { "Master Tracker", "Master Tracker" },
        { "Forest Walker", "Forest Walker" },
        { "Woodsman", "Woodswoman" },

        { "Master Woodsman", "Mistress Woodswoman" },
        { "Ranger Initiate", "Ranger Initiate" },
        { "Ranger Candidate", "Ranger Candidate" },
        { "Ranger Squire", "Ranger Squire" },
        { "Apprentice Ranger", "Apprentice Ranger" },

        { "Ranger", "Ranger" },
        { "Greater Ranger", "Greater Ranger" },
        { "Ranger Captain", "Ranger Captain" },
        { "Ranger General", "Ranger General" },
        { "Master Ranger", "Mistress Ranger" },

        { "Ranger Lord", "Ranger Lady" },
        { "Ranger Baron", "Ranger Baroness" },
        { "Ranger Prince", "Ranger Princess" },
        { "Ranger King", "Ranger Queen" },
        { "Warder", "Warder" },

        { "Warder Captain", "Warder Captain" },
        { "Warder General", "Warder General" },
        { "Master of Warders", "Master of Warders" },
        { "Lord of Warders", "Lady of Warders" },
        { "Warder Baron", "Warder Baroness" },

        { "Warder Prince", "Warder Princess" },
        { "Warder King", "Warder King" },
        { "Warrior of the Forest", "Warrior of the Forest" },
        { "Sword of the Forest", "Sword of the Forest" },
        { "Knight of the Forest", "Knight of the Forest" },

        { "Master of the Forest", "Mistress of the Forest" },
        { "Lord of the Forest", "Lady of the Forest" },
        { "Baron of the Forest", "Baron of the Forest" },
        { "Prince of the Forest", "Princess of the Forest" },
        { "King of the Forest", "Queen of the Forest" },

        { "Watcher of the Glade", "Watcher of the Glade" },
        { "Defender of the Glade", "Defender of the Glade" },
        { "Keeper of the Glade", "Keeper of the Glade" },
        { "Preserver of Nature", "Preserver of Nature" },
        { "Supreme Protector of Nature", "Supreme Protector of Nature" },

        { "Ranger Hero",                  "Ranger Heroine"                  },
        { "Avatar",            "Avatar"               },
        { "Angel",             "Angel"                },
        { "Demigod",           "Demigoddess"          },
        { "Immortal",          "Immortal"             },
        { "God",               "Goddess"              },
        { "Deity",             "Deity"                },
        { "Supremity",         "Supremity"            },
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

        {
        { "Man",        "Woman"                         },
        { "Novice",     "Novice",                       },      /* 1 */
        { "White Belt", "White Belt"                    },
        { "Monastic",   "Monastic"                      },
        { "Thoughtful", "Thoughtful"                    },
        { "Novice pugilist",    "Novice Pugilist"       }, /* 5 */
        { "Scholar of Combat",  "Scholar of Combat"     },
        { "Yellow Belt",        "Yellow Belt"           },
        { "Learned pugilist",   "Learned Pugilist"      },
        { "Scholar of War",     "Scholar of War"        },
        { "Unarmed Fighter",    "Unarmed Fighter"       },      /* 10 */
        { "Orange Belt",        "Orange Belt"           },
        { "Boxer",              "Boxer"                 },
        { "Kickboxer",          "Kickboxer"             },
        { "Green Belt",         "Green Belt"            },
        { "Learned Combatant",  "Learned Combatant"     }, /* 15 */
        { "Blue Belt",          "Blue Belt"             },
        { "Red Belt",           "Red Belt"              },
        { "Novice Monk",        "Novice Monk"           },
        { "Initiate Monk",      "Initiate Monk"         },
        { "Monk",               "Monk"                  },     /* 20 */
        { "Brown Belt",         "Brown Belt"            },
        { "Expert Pugilist",    "Expert Pugilist"       },
        { "2nd Brown",          "2nd Brown"             },
        { "Master Pugilist",    "Mistress Pugilist"     },
        { "Black Belt",         "Black Belt"            },      /* 25 */
        { "Second Dan",         "Second Dan"            },
        { "Third Dan",          "Third Dan"             },
        { "Master of Akemi",    "Mistress of Akemi"     },
        { "Master of Patience", "Mistress of Patience"  },
        { "Master of Thought",  "Mistress of Thought"   },  /* 30 */
        { "Master of Mind",     "Mistress of Mind"      },
        { "Master of Combat",   "Mistress of Combat"    },
        { "Master of Unarmed Combat", "Mistress of Unarmed Combat"},
        { "Tranquil",           "Tranquil",             },
        { "Master of Shuto",    "Mistress of Shuto"     },      /* 35 */
        { "Enlightened",        "Enlightened"           },
        { "Lore Master",        "Lore Mistress"         },
        { "Adept",              "Adept"                 },
        { "Adept of Wind",      "Adept of Wind"         },
        { "Adept of Mind",      "Adept of Mind"         },      /* 40 */
        { "Adept of Thought",   "Adept of Thought"      },
        { "Adept of State",     "Adept of State"        },
        { "Kaiden",             "Kaiden"                },
        { "Sensei",             "Sensei"                },
        { "Sensei",             "Sensei"                },      /* 45 */
        { "Shihan",             "Shihan"                },
        { "Archon of Thought",  "Archon of Thought"     },
        { "Archon of State",    "Archon of State"       },
        { "Transcendant",       "Transcendant"          },
        { "Lord Adept",         "Lady Adept"            },  /* 50 */
        { "Monk Hero",          "Monk Heroine"          },
	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},
{"Apprentice Channeler", "Apprentice Channeler"},
{"Student of Channeling",
"Student of Channeling"},
{"Journeyman Channeler", "Journeywoman Channeler"},
{"Novice Channeler","Novice Channeler"},
{"Medium of Channeling","Medium of Channeling"},
{"Apprentice Scrivener","Apprentice Scrivener"},
{"Journeyman Scrivener", "Journeywoman Scrivener"},
{"Scrivener","Scrivener"},
{"Accomplished Scrivener","Accomplished Scrivener"},
{"Master Scribe","Master Scribe"},
{"Apprentice Alchemist","Apprentice Alchemist"},
{"Student of Alchemy","Student of Alchemy "},
{"Journeyman Alchemist", "Jourenywoman Alchemist"},
{"Alchemist","Alchemist"},
{"Master of Alchemy","Master of Alchemy"},
{"Wizard", "Witch"},
{"Sorcerer","Sorceress"},
{"Grand Sorcerer","Grand Sorceress"},
{"Master of Sorcery", "Mistress of Sorcery"},
{"Channeler","Channeler"},
{"Maker of Stones","Maker of Stones"},
{"Maker of Wands","Maker of Wands"},
{"Maker of Potions","Maker of Potions"},
{"Maker of Staves","Maker of Staves"},
{"Craftsman", "Craftswoman",},
{"Master Craftsman", "Mistress Craftswoman"},
{"Student of Light","Student of Light"},
{"Student of Focus","Student of Focus"},
{"Student of Absorption","Student of Absorption"},
{"Student of Form","Student of Form"},
{"Student of Space","Student of Space"},
{"Student of Time","Student of Time"},
{"Wizard of Light","Wizard of Light"},
{"Wizard of Focus","Wizard of Focus"},
{"Wizard of Absorption","Wizard of Absorption"},
{"Wizard of Form","Wizard of Form"},
{"Wizard of Space","Wizard of Space"},
{"Temporal Wizard","Temporal Wizard"},
{"Temporal Master", "Temporal Mistress"},
{"Mystic","Mystic"},
{"Grand Mystic","Grand Mystic"},
{"Master Mystic", "Mistress Mystic"},
{"Archmage of Light","Archmage of Light"},
{"Archmage of Focus","Archmage of Focus"},
{"Archmage of Absorption","Archmage of Absorption"},
{"Archmage of Form","Archmage of Form"},
{"Archmage of Space","Archmage of Space"},
{"Temporal Archmage","Temporal Archmage"},
{"Archmage of Mystism","Archmage of Mystism"},
{"Archmage of Channeling","Archmage of Channeling"},
{"Hero of Magical Transferance", "Heroine of Magical Transferance"},

	{ "Avatar of Magic",		"Avatar of Magic"		},
	{ "Angel of Magic",		"Angel of Magic"		},
	{ "Demigod of Magic",		"Demigoddess of Magic"		},
	{ "Immortal of Magic",		"Immortal of Magic"		},
	{ "God of Magic",		"Goddess of Magic"		},
	{ "Deity of Magic",		"Deity of Magic"		},
	{ "Supremity of Magic",		"Supremity of Magic"		},
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

        { "Uke", "Uke"  },
        { "Tori", "Tori"        },
        { "White Belt", "White Belt"    },
        { "Yellow Belt", "Yellow Belt"  },
        { "Green Belt", "Green Belt"    },

        { "3rd Brown", "3rd Brown"      },
        { "2nd Brown", "2nd Brown"      },
        { "1st Brown", "1st Brown"      },
        { "3rd Black", "3rd Black"      },
        { "2nd Black", "2nd Black"      },

        { "Okuri", "Okuri"      },
        { "Shuto", "Shuto"      },
        { "Uraken", "Uraken"    },
        { "Hidden", "Hidden"    },
        { "Shrouded", "Shrouded"        },

        { "Prowler", "Prowler"  },
        { "Tailer", "Tailer"    },
        { "Stalker", "Stalker"  },
        { "Master Stalker", "Master Stalker"    },
        { "Nightwalker", "Nightwalker"  },

        { "Master of the Shadows", "Mistress of the Shadows"    },
        { "Master of the Darkness", "Mistress of the Darkness"  },
        { "Master of the Night", "Mistress of the Night"  },
        { "King of Black", "King of Black"      },
        { "Shadowwalker", "Shadowwalker"        },

        { "Master of Stealth", "Mistress of Stealth"      },
        { "Master of Initiative", "Mistress of Initiative"      },
        { "Master of Surprise", "Mistress of Surprise"  },
        { "Contracted Killer", "Contracted Killer"      },
        { "Assassin", "Assassin"        },

        { "Master of Guile", "Mistress of Guile"  },
        { "Master of Severance", "Mistress of Severance"        },
        { "Master of the Blade", "Mistress of the Blade"        },
        { "Mad Knifer", "Mad Knifer"    },
        { "Slasher", "Slasher"  },

        { "Lord of the Night", "Lady of the Night"      },
        { "Prince of the Night", "Princess of the Night"        },
        { "King of the Night", "Queen of the Night"     },
        { "Overlord of Darkness", "Overlord of Darkness"        },
        { "Malevolent", "Malevolent"    },

        { "Sadist", "Sadist"    },
        { "Malignant", "Malignant"      },
        { "Conspirator", "Conspirator"  },
        { "Despised", "Despised"        },
        { "Forsaken", "Forsaken"        },

        { "Unseen", "Unseen"    },
        { "Unheard", "Unheard"  },
        { "Unknown", "Unknown"  },
        { "Unspeakable Thing", "Unspeakable Thing"      },
        { "Unholy Terror", "Unholy Terror"      },


        { "Hero of the Dark",           "Heroine of the Dark"                  },
        { "Avatar",            "Avatar"               },
        { "Angel",             "Angel"                },
        { "Demigod",           "Demigoddess"          },
        { "Immortal",          "Immortal"             },
        { "God",               "Goddess"              },
        { "Deity",             "Deity"                },
        { "Supremity",         "Supremity"            },
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

        { "Apprentice of Black Magic", "Apprentice of Black Magic" },
        { "Student of Dark Spells", "Student of Dark Spells" },
        { "Scholar of Dark Magic", "Scholar of Dark Magic" },
        { "Delver in Dark Spells", "Delver in Dark Spells" },
        { "Channeler of Dark Forces", "Channeler of Dark Forces" },

        { "Scribe of Black Magic", "Scribe of Black Magic" },
        { "Dark Seer", "Dark Seer" },
        { "Dark Sage", "Dark Sage" },
        { "Apprentice of Dark Illusions", "Apprentice of Dark Illusions" },
        { "Student of Dark Illusions", "Student of Dark Illusions" },

        { "Dark Illusionist", "Dark Illusionist" },
        { "Master of Dark Illusions", "Mistress of Dark Illusions" },
        { "Conjurer of Evil", "Conjurer of Evil" },
        { "Evil Magician", "Evil Magician" },
        { "Evil Creator", "Evil Creator" },

        { "Evil Savant", "Evil Savant" },
        { "Warlock", "Witch" },
        { "Sorcerer", "Sorceress" },
        { "Grand Sorcerer", "Grand Sorceress" },
        { "Apprentice Golem Maker", "Apprentice Golem Maker" },

        { "Golem Maker", "Golem Maker" },
        { "Creator of Disease", "Creator of Disease" },
        { "Hurler of Acid", "Hurler of Acid" },
        { "Ghostly Illusionist", "Ghostly Illusionist" },
        { "Apprentice Animator", "Apprentice Animator" },

        { "Animator", "Animator" },
        { "Archmage of Animation", "Archmage of Animation" },
        { "Bringer of Plague", "Bringer of Plague" },
        { "Necromancer", "Necromancer" },
        { "Apprentice Summoner", "Apprentice Summoner" },

        { "Summoner", "Summoner" },
        { "Archmage of Summoning", "Archmage of Summoning" },
        { "Apprentice Summoner of Demons", "Apprentice Summoner of Demons" },
        { "Summoner of Demons", "Summoner of Demons" },
        { "Archmage Summoner of Demons", "Archmage Summoner of Demons" },

        { "Abominable Archmage", "Abominable Archmage" },
        { "Malefic Archmage", "Malefic Archmage" },
        { "Infernal Archmage", "Infernal Archmage" },
        { "Malevolent Archmage", "Malevolent Archmage" },
        { "Apprentice Occultist", "Apprentice Occultist" },

        { "Student of the Occult", "Student of the Occult" },
        { "Occultist", "Occultist" },
        { "Archmage of the Occult", "Archmage of the Occult" },
        { "Student of Black Magic", "Student of Black Magic" },
        { "Archmage of Black Magic", "Archmage of Black Magic" },

        { "Archmage of Necromancy", "Archmage of Necromancy" },
        { "Spectral Archmage", "Spectral Archmage" },
        { "Archmagical Wraith", "Archmagical Wraith" },
        { "Lich", "Lich" },
        { "Lich Lord", "Lich Lady" },

        { "Anti-Hero of Death",              "Anti-Heroine of Death"      },
        { "Avatar",            "Avatar"               },
        { "Angel",             "Angel"                },
        { "Demigod",           "Demigoddess"          },
        { "Immortal",          "Immortal"             },
        { "God",               "Goddess"              },
        { "Deity",             "Deity"                },
        { "Supremity",         "Supremity"            },
	{ "Creator",			"Creator"			},
	{ "Implementor",		"Implementress"			}
    },

    {
	{ "Man",			"Woman"				},

        { "Apprentice of Magic", "Apprentice of Magic" },
        { "Spell Student", "Spell Student" },
        { "Scholar of Magic", "Scholar of Magic" },
        { "Delver in Spells", "Delver in Spells" },
        { "Medium of Magic", "Medium of Magic" },

        { "Scribe of Magic", "Scribe of Magic" },
        { "Seer", "Seer" },
        { "Sage", "Sage" },
        { "Illusionist", "Illusionist" },
        { "Abjurer", "Abjurer" },

        { "Invoker", "Invoker" },
        { "Conjurer", "Conjurer" },
        { "Magician", "Magician" },
        { "Creator", "Creator" },
        { "Savant", "Savant" },

        { "Magus", "Magus" },
        { "Wizard", "Witch" },
        { "Warlock", "War Witch" },
        { "Elemental Apprentice", "Elemental Apprentice" },
        { "Elemental Magician", "Elemental Magician" },

        { "Student of the Elements", "Student of the Elements" },
        { "Student of Fire", "Student of Fire" },
        { "Student of Ice", "Student of Ice" },
        { "Student of Earth", "Student of Earth" },
        { "Student of Water", "Student of Water" },

        { "Student of Air", "Student of Air" },
        { "Student of Ether", "Student of Ether" },
        { "Student of Conjuration", "Student of Conjuration" },
        { "Student of Abjuration", "Student of Abjuration" },
        { "Elder Elemental Magician", "Elder Elemental Magician" },

        { "Learned of the Elements", "Learned of the Elements" },
        { "Master of Fire", "Mistress of Fire" },
        { "Master of Ice", "Mistress of Ice" },
        { "Master of Earth", "Mistress of Earth" },
        { "Master of Water", "Mistress of Water" },

        { "Master of Air", "Mistress of Air" },
        { "Master of Ether", "Mistress of Ether" },
        { "Master of Conjuration", "Mistress of Conjuration" },
        { "Master of Abjuration", "Mistress of Abjuration" },
        { "Master of the Elements", "Mistress of the Elements" },

        { "Greater Elemental Magician", "Greater Elemental Magician" },
        { "Master of Alteration", "Mistress of Alteration" },
        { "Master of Evocation", "Mistress of Evocation" },
        { "Master of Creation", "Mistress of Creation" },
        { "Grand Elemental Magician", "Grand Elemental Magician" },

        { "Commander of Air", "Commander of Air" },
        { "Summoner of Water", "Summoner of Water" },
        { "Caller of Fire", "Caller of Fire" },
        { "Wielder of Earth", "Wielder of Earth" },
        { "Grand Master of the Elements", "Grand Mistress of the Elements" },

        { "Elemental Hero",             "Elemental Heroine"                  },
        { "Avatar",                "Avatar"                   },
	{ "Angel",			"Angel"				},
	{ "Demigod",			"Demigoddess",			},
	{ "Immortal",			"Immortal"			},
	{ "God",			"Goddess"			},
	{ "Deity",			"Deity"				},
	{ "Supreme Master",		"Supreme Mistress"		},
        { "Creator",                    "Creator"                       },
	{ "Implementor",		"Implementress"			}
    }
};



/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0, 100,  7 },
    {  0,  0, 150,  8 },
    {  0,  0, 180,  9 },
    {  0,  0, 200, 10 }, /* 10  */
    {  0,  0, 215, 11 },
    {  0,  0, 230, 12 },
    {  0,  0, 230, 13 }, /* 13  */
    {  0,  1, 240, 14 },
    {  1,  1, 250, 15 }, /* 15  */
    {  1,  2, 265, 16 },
    {  2,  3, 280, 22 },
    {  2,  3, 300, 25 }, /* 18  */
    {  3,  4, 325, 30 },
    {  3,  5, 350, 35 }, /* 20  */
    {  4,  6, 400, 40 },
    {  4,  6, 450, 45 },
    {  5,  7, 500, 50 },
    {  5,  8, 550, 55 },
    {  6,  9, 600, 60 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 80 },
    { 85 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 1 },	/* 10 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 2 },	/* 15 */
    { 2 },
    { 2 },
    { 3 },	/* 18 */
    { 3 },
    { 3 },	/* 20 */
    { 3 },
    { 4 },
    { 4 },
    { 4 },
    { 5 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60,0 },   /* 0 */
    {   50,0 },   /* 1 */
    {   50,0 },
    {   40,0 },
    {   30,0 },
    {   20,0 },   /* 5 */
    {   10,0 },
    {    0,0 },
    {    0,0 },
    {    0,0 },
    {    0,0 },   /* 10 */
    {    0,1 },
    {    0,1 },
    {    0,1 },
    {    0,2 },
    { - 10,2 },   /* 15 */
    { - 15,2 },
    { - 20,3 },
    { - 30,3 },
    { - 40,4 },
    { - 50,4 },   /* 20 */
    { - 60,5 },
    { - 75,6 },
    { - 90,7 },
    { -105,8 },
    { -120,9 }    /* 25 */
};


const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 }    /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	}, 
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   }, 
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },	
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
        "reserved",             { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
        { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			"",		""
    },

    {
	"absorb",		{ 53, 53, 53, 53, 53, 53, 53, 33, 53, 53,53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_absorb,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_absorb,		SLOT(551),	100,	12,
	"",			"The shimmering fields around you fades.",
	"$p's shimmering field fades."
    },

    {
        "acid blast",           { 53, 53, 53, 53, 53, 53, 53, 53, 53, 28, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	20,	12,
	"acid blast",		"!Acid Blast!"
    },

    {
	"embalm",	{53, 53, 53, 53, 53, 53, 53, 53, 53, 15, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_embalm,	SLOT( 0),	20,	12,
	"",	"",""
    },

    {
	"trace",	{25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_trace,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(731),	60,	12,
	"",	"",	""
    },

    {
	"flame scorch",	{53, 53,53,53,53,53,53,53,53,53,53},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_flame_scorch,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_flame_scorch,	SLOT(912),	50,	12,
	"searing blue-green flame",	"",	""
    },

        {
        "soulbind",     {61,61,61,61,61,61,61,61,61,40,61},
	{1,1,1,1,1,1,1,1,1,1,1},
        spell_soulbind,      TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,   SLOT(913),      100,    12,
        "",     "",     "",
        },

        {
        "shattered bone",       {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,     TAR_IGNORE,     POS_DEAD,
        &gsn_shattered_bone,    SLOT(0),0,      0,
        "fragments of blasted bone",    "",     ""
        },

        {
        "blood tide",   {61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0},
        spell_null,     TAR_IGNORE,     POS_DEAD,
        &gsn_blood_tide,        SLOT(0), 	0,      0,
        "bloody tide", "",     "",
        },
/* const.c skill table stuff for flesh-cutter */
{
        "mutilated left hand",  {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,            TAR_IGNORE,     POS_DEAD,
        &gsn_mutilated_left_hand,       SLOT(0),        0,      0,
        "",     "The feeling in your mangled left hand returns.",
        ""
},

{
        "mutilated right hand",  {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,            TAR_IGNORE,     POS_DEAD,
        &gsn_mutilated_right_hand,       SLOT(0),        0,      0,
        "",     "The feeling in your mangled right hand returns.",
        ""
},

{
        "mutilated left arm",  {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,          TAR_IGNORE,     POS_DEAD,
        &gsn_mutilated_left_arm,       SLOT(0),        0,      0,
        "",     "The feeling in your mangled left arm returns.",
        ""
},

{
        "mutilated right arm",  {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,       TAR_IGNORE,     POS_DEAD,
        &gsn_mutilated_right_arm,       SLOT(0),        0,      0,
        "",     "The feeling in your mangled right arm returns.",
        ""
},

{
        "mutilated left leg",  {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,           TAR_IGNORE,     POS_DEAD,
        &gsn_mutilated_left_leg,       SLOT(0),        0,      0,
        "",     "The feeling in your mangled left leg returns.",
        ""
},

{
        "mutilated right leg",  {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,            TAR_IGNORE,     POS_DEAD,
        &gsn_mutilated_right_leg,       SLOT(0),        0,      0,
        "",     "The feeling in your mangled right leg returns.",
        ""
},




    {
	"talk to dead",	{52, 52, 18, 24, 52, 52, 52, 52, 52, 21, 52},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_talk_to_dead,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(730),	20,	12,
	"",	"",	""
    },


{
        "deathspell",   {53, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_deathspell,       TAR_IGNORE, POS_STANDING,
        NULL,   SLOT(740),      150,    12,
        "explosion of negative energy",  ""
},

{
        "trinal wield", {25, 30, 53, 30, 32, 53, 53, 53, 53, 53, 53},
        { 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_tertiary_wield,    SLOT(0),        0,      0,
        "",     "",	""
},

{
        "forest blending",      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_forest_blending,   SLOT(0),        0,      0,
        "",     "",	""
},

{
        "awareness",      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_awareness,   SLOT(0),        0,      0,
        "",     "Your awareness of the surrounding trees lessens."
},

{
	"door bash",	{18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_door_bash,	SLOT(0),	0,	0,
	"failed door bashing",	"",	""
},

{
        "gaseous form", {25, 25, 25, 25, 25, 25, 25, 25, 25, 25,25},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_gaseous_form,      SLOT(0),        0,      0,
        "",     "You come out of gaseous form."
},

{
        "breath fire",  {20,20,20,20,20,20,20,20,20,20,20},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_breath_fire,       SLOT(0),        0,      0,
        "blast of flame",
        "Your body once more has retained enough internal heat to breath fire again."
},
 {
	"regeneration", {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_regeneration,	SLOT(0),	0,	0,
	"",	"",	""
},
{
	"brand",{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0},
	spell_null,	TAR_IGNORE, POS_DEAD,
	&gsn_wolverine_brand,	SLOT(0),	0,	0,
	"",	"The fire coursing through your blood cools down.", ""
},
{
	"silence",{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_silence,	SLOT(0),	0,	0,
	"",	"You feel the words come back to you.", ""
},
{
	"brand",	{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_parrot,	SLOT(0),	0,	0,
	"brand of the silent parrot",	"You feel once again ready to call upon your god.",	""
},

{
	"hunger",	{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_starvation,	SLOT(0),	0,	0,
	"starvation",	"You no longer feel the pangs of famine."
},

{
	"dehydrated",	{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_null,	TAR_IGNORE,	POS_DEAD,
	&gsn_dehydrated,	SLOT(0),	0,	0,
	"dehydration",	"You no longer feel so thirsty."
},


{
        "rear kick",    {18, 53, 27, 23, 53, 20, 32, 32, 32, 32, 32},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_rear_kick, SLOT(0),        0,      0,
        "rear kick",     ""
},

{
	"spike",	{52,52,52,29,52,52,52,52,52,52,52},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_spike,	SLOT(0),	0,	0,
	"spike",	""
},

{
        "lore", {10,10,10,10,10,10,10,10,10,10,10},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_RESTING,
        &gsn_lore,      SLOT(0),        0,      0,
        "",     ""
},

{
	"shapeshift",	{10,10,10,10,10,10,10,10,10,10,10},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_shapeshift,	SLOT(0),	0,	0,
	"",	"You resume your natural form."
},

{
	"globe of darkness",	{20,20,20,20,20,20,20,20,20,20,20},
	{1,1,1,1,1,1,1,1,1,1,1,},
	spell_globe_of_darkness,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(780),	100,	12,
	"",	
	"Your power to call upon the darkness returns.", ""
},
{
        "animate object",       {53,53,53,53,53,53,53,35,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_animate_object,   TAR_OBJ_INV,    POS_STANDING,
        NULL,   SLOT(741),      100,    12,
        "",     ""
},

{
        "lifebane",     {53,53,53,53,53,53,53,53,34,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_lifebane,        TAR_IGNORE,     POS_FIGHTING,
        NULL,   SLOT(742),      45,     12,
        "lifebane",     ""
},

{
        "demand",       {53,53,53,53,30,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_demand,    SLOT(0),        0,      12,
        "",     ""
},



    {
	"animate dead",		{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 25, 53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_animate_dead,	TAR_IGNORE,		POS_STANDING,
	&gsn_animate_dead,			SLOT(570),	100, 0,
	"",			"Your power to raise dead returns.",	""
    },

    {
	"lesser golem",	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lesser_golem, TAR_IGNORE, POS_STANDING,
	NULL,	SLOT(591),	70,	24,
	"",	"You feel up to fashioning another golem.", ""
    },

    {
	"greater golem",	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 33, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_greater_golem, TAR_IGNORE, POS_STANDING,
	NULL,	SLOT(591),	70,	24,
	"",	"You feel up to fashioning another iron golem.", ""
    },

    {
        "armor",                { 53, 53, 1, 5, 5, 7, 1, 1, 8, 1, 1 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less armored.",	""
    },

    {
        "bless",                { 53, 53, 3, 4, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_bless,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous.", 
	"$p's holy aura fades."
    },

    {
        "blindness",            { 53, 53, 11, 53, 9, 53, 53, 53, 12, 12, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again.",	""
    },

    {
        "burning hands",        { 53, 53, 53, 53, 53, 53, 53, 53, 53,53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	""
    },

    {
        "call lightning",       { 53, 53, 27, 53, 53, 22, 53, 53, 53, 53, 16 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!",	""
    },

    {
	"call slayer", {53,53,53,53,53,53,53,53,53,53,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_call_slayer,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	SLOT(906),	25,12,
	"",	"",	""
    },
	
    {   "calm",                 { 53, 53, 17, 14, 53, 53, 53, 53, 53, 53, 14 },
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_calm,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.",	""
    },

    {
        "cancellation",         { 53, 53, 19, 53, 53, 53, 53, 16, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cancellation,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	""			"!cancellation!",	""
    },

    {
        "cause critical",       { 53, 53, 18, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	""
    },

    {
        "cause light",          { 53, 53, 3, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	""
    },

    {
        "cause serious",        { 53, 53, 12, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	""
    },

    {   
        "chain lightning",      { 53, 53, 53, 53, 53, 53, 53, 26, 53, 53, 29 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	""
    }, 

    {
        "change sex",           { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(82),	15,	12,
	"",			"Your body feels familiar again.",	""
    },

    {
        "charm person",         { 53, 53, 53, 53, 34, 53, 53, 53, 53, 53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",			"You feel more self-confident.",	""
    },

    {
        "chill touch",          { 53, 53, 53, 53, 3, 53, 53, 53, 53, 3, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	""
    },

    {
        "colour spray",         { 53, 53, 53, 53, 53, 53, 53, 13, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!",	""
    },

    {
	"firestream",	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 15},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_firestream,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,	SLOT(565),	20,	12,
	"firestream",	"",	""
    },

    {
	"fireshield",	{53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fireshield,	TAR_IGNORE,	POS_STANDING,
	&gsn_fireshield,	SLOT(617),	60,	12,
	"",	"Your flaming shield dissipates.",	""
    },
    {
	"iceshield",	{53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_iceshield,	TAR_IGNORE,	POS_STANDING,
	&gsn_iceshield,	SLOT(618),	60,	12,
	"",	"Your frost shield dissipates.",	""
    },

    {
	"concatenate",		{ 53, 53, 53, 53, 53, 53, 53, 29, 53, 53,53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_concatenate,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(568),	30,	12,
	"blast of energy",	"!Concatenate!",	""
    },

    {
	"heavenly wrath",	{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_heavenly_sceptre_frenzy,	TAR_CHAR_SELF,	POS_FIGHTING,
	NULL,	SLOT(900),	0,	0,
	"",	"The heavenly wrath bestowed upon you fades away.",	""
    },

    {
	"heavenly fire",	{61,61,61,61,61,61,61,61,61,61,61},
	{0,0,0,0,0,0,0,0,0,0,0},
	spell_heavenly_sceptre_fire,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	SLOT(901),	0,	0,
	"heavenly fire",	"You feel the power drained from you by the scetpre's heavenly fire return.",	""
    },

    {
	"wrath",		{ 53, 53, 53, 21, 53, 53, 53, 53, 53, 53, 53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_wrath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(553),	20,	12,
	"heavenly wrath",	"!wrath!",	""
    },

    {
        "continual light",      { 53, 53, 53, 53, 53, 53, 53, 12, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	""
    },

    {
        "control weather",      { 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 14 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	""
    },

    {
        "create food",          { 53, 53, 7, 11, 53, 53, 53, 12, 53, 53,9 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	""
    },

    {
        "create rose",          { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_rose,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	""
    },  

    {
        "create spring",        { 53, 53, 16, 18, 53, 53, 53, 53, 53, 53,9 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	""
    },

    {
        "create water",         { 53, 53, 8, 12, 53, 53, 53, 9, 53, 53, 8},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	""
    },

    {
        "cure blindness",       { 53, 53, 13, 15, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	""
    },

    {
        "cure critical",        { 53, 53, 18, 20, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(15),	20,	12,
	"",			"!Cure Critical!",	""
    },

    {
        "cure disease",         { 53, 53, 15, 18, 53, 53, 28, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	"",			"!Cure Disease!",	""
    },

    {
        "cure light",           { 53, 53, 3, 6, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(16),	10,	12,
	"",			"!Cure Light!",		""
    },

    {
        "cure poison",          { 53, 53, 13, 19, 53, 53, 24, 53, 53, 53, 16 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	""
    },

    {
        "cure serious",         { 53, 53, 9, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!Cure Serious!",	""
    },

    {
        "curse",                { 53, 53, 20, 53, 17, 53, 53, 53, 53, 16,
53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_curse,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off.", 
	"$p is no longer impure."
    },

    {
	"dark wrath",		{ 53, 53, 53, 53, 53, 53, 53, 53, 22, 53, 53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_dark_wrath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(550),	20,	12,
	"unholy dark wrath",	"!DarkWrath!",
    },

    {
        "demonfire",            { 53, 53, 53, 53, 53, 53, 53, 53, 33, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_demonfire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(505),	20,	12,
	"torments",		"!Demonfire!",		""
    },	

    {
        "detect evil",          { 53, 53, 53, 10, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears.",	""
    },

    {
        "detect good",          { 53, 53, 53, 53, 10, 53, 53, 53, 53, 2, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_detect_good,      TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
        "",                     "The gold in your vision disappears.",	""
    },

    {
        "detect hidden",        { 53, 15, 53, 53, 53, 53, 18, 53, 16,53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	 	TAR_IGNORE,		POS_STANDING,
	&gsn_detect_hidden,	SLOT(0),	 5,	12,
	"",			"You feel less aware of your surroundings.",	
	""
    },

    {
	"true sight",	{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_true_sight,	TAR_CHAR_SELF,	POS_STANDING,
	&gsn_true_sight,	SLOT( 0),	25,	12,
	"",	"Your vision dulls and you see less truly.", ""
    },

    {
        "detect invis",         { 53, 53, 9, 10, 9, 53, 7, 8, 12, 8, 10},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects.",
	""
    },

    {
        "detect magic",         { 53, 53, 4, 53, 53, 53, 53, 1, 53, 53, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"The detect magic wears off.",	""
    },

    {
        "detect poison",        { 53, 53, 8, 53, 53, 53, 53, 53, 8, 53, 4 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	""
    },

    {
        "dispel evil",          { 53, 53, 53, 7, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!",	""
    },

    {
        "dispel good",          { 53, 53, 53, 53, 7, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
        "dispel good",          "!Dispel Good!",	""
    },

    {
        "dispel magic",         { 53, 53, 20, 53, 53, 53, 53, 9, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	""
    },

    {
        "earthquake",           { 53, 53, 15, 53, 53, 53, 53, 53, 53, 53, 10 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		""
    },

    {
        "enchant armor",        { 53, 53, 53, 53, 53, 53, 53, 17, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_enchant_armor,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	""
    },

    {
        "enchant weapon",       { 53, 53, 53, 53, 53, 53, 53, 20, 53, 53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	""
    },

    {
        "energy drain",         { 53, 53, 53, 53, 27, 53, 53, 53, 30, 19, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	50,	12,
	"energy drain",		"The cold chll in your body fades away.",
""
    },

    {
   	"evil eye",		{ 53, 53, 53, 53, 53, 53, 53, 53, 37, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_evil_eye,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,			SLOT(559),	150,	24,
	"deathly gaze",		"!EvilEye!",		"",
    },

    {
        "faerie fire",          { 53, 53, 11, 53, 6, 53, 3, 5, 4, 3, 5 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	""
    },

    {
        "faerie fog",           { 53, 53, 14, 53, 53, 53, 53, 16, 53,17,17 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"The fog on your body fades away.",
""
    },


    {
	"unholy fire", {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_unholy_fire,	TAR_IGNORE, POS_FIGHTING,
	NULL,	SLOT(700),	50,	12,
	"unholy fire",	"",	""
    },
    {
	"acid spit", {53, 53, 53, 53, 53, 53, 53, 53 ,53 ,53, 53},
	{ 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1},
	spell_acid_spit,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_spit,	SLOT(701),	15,	12,
	"acidic spittle",	"The acid in your eyes washes out.", ""
    },

    {
        "fireball",             { 53, 53, 53, 53, 22, 53, 53, 27, 53, 53, 27 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fireball,		TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!",		""
    },
  
    {
        "fireproof",            { 53, 53, 53, 53, 53, 53, 24, 25, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fireproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",			"",	"$p's protective aura fades."
    },

    {
        "flamestrike",          { 53, 53, 53, 15, 53, 53, 53, 53, 53, 17, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		""
    },

    {
        "fly",                  { 53, 53, 21, 53, 53, 53, 53, 18, 53, 53,16 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(56),	10,	18,
	"",			"You slowly float to the ground.",	""
    },

    {
        "frenzy",               { 53, 53, 25, 53, 53, 53, 53, 53, 53, 
53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_frenzy,           TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Your rage ebbs.",	""
    },

    {
        "gate",                 { 53, 53, 20, 53, 53, 53, 53, 24, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		""
    },

    {
        "giant strength",       { 53, 53, 29, 53, 53, 53, 53, 24, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(39),	20,	12,
	"",			"You feel weaker.",	""
    },

    {
        "harm",                 { 53, 53, 24, 53, 53, 53, 53, 53, 53, 53, 
53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!,		"""
    },
  
    {
        "haste",                { 53, 53, 53, 53, 53, 53, 26, 23, 53, 53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	""
    },

    {
        "heal",                 { 53, 53, 26, 29, 53, 53, 53, 53, 53, 53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		""
    },
  
    {
        "heat metal",           { 53, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
	"spell",		"!Heat Metal!",		""
    },

    {
	"curse weapon",	{53, 53, 53, 53, 28, 53, 53, 53, 30, 25,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_curse_weapon,	TAR_OBJ_INV,	POS_STANDING,
	NULL,	SLOT(771),	75,	12,
	"",	"", ""
    },

    {
	"bless weapon",	{53, 53, 53, 41, 53, 53, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_bless_weapon,	TAR_OBJ_INV,	POS_STANDING,
	NULL,	SLOT(621),	150,	24,
	"",	"",	"",
    },

    {
        "holy word",            { 53, 53, 40, 40, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_holy_word,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
	"divine wrath",		"!Holy Word!",		""
    },

    {
        "identify",             { 53, 53, 14, 16, 16, 53, 19, 15, 53, 14,15 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	18,
	"",			"!Identify!",		""
    },

    {
        "infravision",          { 53, 53, 10, 53, 14, 53, 7, 53, 4, 6, 8},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
	"",			"You no longer see in the dark.",	""
    },

    {
        "invisibility",         { 53, 53, 53, 53, 12, 53, 12, 8, 13, 8, 10 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_invis,		TAR_OBJ_CHAR_DEF,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",		
	"$p fades into view."
    },

    {
        "know alignment",       { 53, 53, 15, 18, 53, 53, 53, 53, 53,53,53
},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	""
    },
/*
    {
        "transform",   	        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_transform,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(555),	125,	24,
	"",	"You feel your health return to normal.",	""
    },
*/
    {
	"brew",	{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_brew,	TAR_OBJ_INV,	POS_STANDING,
	NULL,	SLOT(598),	35,	12,
	"",	"",	""
    },

    {
        "lightning bolt",       { 53, 53, 53, 53, 16, 53, 53, 9, 53, 53,53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(30),	15,	12,
	"lightning bolt",	"!Lightning Bolt!",	""
    },

    {
        "locate object",        { 53, 53, 20, 20, 53, 53, 29, 20,53,20,20},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	""
    },

    {
        "magic missile",        { 53, 53, 53, 53, 1, 53, 53, 1, 53, 1, 
1 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(32),	15,	12,
	"magic missile",	"!Magic Missile!",	""
    },

    {
        "mass healing",         { 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_mass_healing,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	""
    },

    {
        "mass invis",           { 53, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"You are no longer invisible.",		""
    },

    {
        "pass door",            { 53, 53, 36, 53, 53, 53, 38, 30, 53,23,25 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again.",	""
    },

    {
        "plague",               { 53, 53, 53, 53, 26, 53, 53, 53, 53, 25,
53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_plague,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
	"sickness",		"Your sores vanish.",	""
    },

    {
        "poison",               { 53, 53, 53, 53, 27, 53, 53, 53, 53, 17, 
53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_poison,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick.",	
	"The poison on $p dries up."
    },

    {
	"power word kill",	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 38,
53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_power_word_kill,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,			SLOT(557),	150,	0,
	"word of power",	"PowerWordKill!",	""
    },

    {
	"power word stun",	{ 53, 53, 53, 53, 38, 53, 53, 53, 53, 53,
53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_power_word_stun,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_power_word_stun,	SLOT(569),	100,	0,
	"word of power",	"You recover from the shock and stagger about slightly.",	""
    },


    {
        "protection evil",      { 53, 53, 15, 12, 53, 53, 11, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_protection_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	""
    },

    {
        "protection good",      { 53, 53, 15, 53, 21, 53, 53, 53, 18, 15, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_protection_good,  TAR_CHAR_SELF,          POS_STANDING,
        NULL,                   SLOT(514),       5,     12,
        "",                     "You feel less protected.",	""
    },

    {
        "ray of truth",         { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 
53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(518),      20,     12,
        "ray of truth",         "!Ray of Truth!",	""
    },

    {
        "recharge",             { 53, 53, 53, 53, 53, 53, 53, 27, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_recharge,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		""
    },

    {
        "refresh",              { 53, 53, 7, 14, 53, 53, 53, 53, 53, 53, 
9 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
	"refresh",		"!Refresh!",		""
    },

    {
        "remove curse",         { 53, 53, 20, 21, 53, 53, 53, 53, 53, 53, 
53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,	POS_STANDING,
	NULL,			SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	""
    },

    {
        "sanctuary",            { 53, 53, 21, 28, 53, 53, 53, 29, 53, 53,32 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	""
    },

    {
        "shield",               { 53, 53, 27, 53, 53, 27, 26, 25, 53, 53, 25 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(67),	12,	12,
	"",			"Your force shield shimmers then fades away.",
	""
    },

    {
        "shocking grasp",       { 53, 53, 53, 53, 53, 53, 53, 5, 53, 53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	""
    },

    {
        "sleep",                { 53, 53, 53, 53, 18, 53, 53, 53, 16, 16, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired.",	""
    },

    {
        "slow",                 { 53, 53, 53, 53, 53, 53, 22, 25, 53, 53,53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_slow,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(515),      30,     12,
        "",                     "You feel yourself speed up.",	""
    },

    {
        "stone skin",           { 53, 53, 29, 53, 53, 53, 53, 53, 53, 53, 26},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	12,
	"",			"Your skin feels soft again.",	""
    },

    {
        "summon",               { 53, 53, 15, 20, 21, 53, 53, 53, 53, 27, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Summon!",		""
    },

    {
        "teleport",             { 53, 53, 13, 53, 14, 53, 53, 15, 53,15,16 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!",		""
    },

    {
	"turn undead",		{ 53, 53, 25, 28, 53, 53, 53, 53, 53, 53, 53 },
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_turn_undead,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(552),	50,	24,
	"divine intervention",	"!TurnUndead!",		""
    },

    {
	"undead drain",	{53,53,53,53,53,53,53,53,53,53,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_undead_drain,		TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	&gsn_undead_drain, SLOT(809),	60,	12,
	"undead drain",	"You feel your body and mind restored to normal.",
	"",
    },


    {
        "ventriloquate",        { 53, 53, 4, 53, 53, 53, 53, 4, 53, 53, 4 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	""
    },

    {
	"team spirit",	{10, 10, 10, 10, 10, 10 ,10 ,10 ,10, 10, 10},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_team_spirit, 	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(703),	5,	12,
	"energy transference",	"",	""
    },

    {
        "venueport",        { 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_venueport,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(558),	 125,	12,
	"",		"You are once again ready to venueport.",	""
    },

    {
        "weaken",               { 53, 53, 53, 53, 27, 53, 53, 53, 53, 29, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(68),	20,	12,
	"spell",		"You feel stronger.",	""
    },

    {
	"web",			{ 53, 53, 53, 53, 53, 53, 53, 53, 18, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_web,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(560),	20,	12,
	"corrosive webs",	"The sticky strands about your body dissolve.",	""
    },


    {
        "word of recall",       { 53, 53, 28, 31, 53, 53, 34, 29, 53, 33,30 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_word_of_recall,	TAR_CHAR_DEFENSIVE, POS_FIGHTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!",	""
    },


    {
        "summon fire elemental", { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 27},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_summon_fire_elemental,    TAR_IGNORE,     POS_STANDING,
        NULL,           SLOT(572),      50,     12,
        "",     "You feel up to calling another fire elemental.", ""
    },

    {
        "summon earth elemental", { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_summon_earth_elemental,    TAR_IGNORE,     POS_STANDING,
        NULL,           SLOT(573),      50,     12,
        "",     "You feel up to calling another earth elemental.", ""
    },

    {
        "summon air elemental", { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 23},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_summon_air_elemental,    TAR_IGNORE,     POS_STANDING,
        NULL,           SLOT(574),      50,     12,
        "",     "You feel up to calling another air elemental.", ""
    },

    {
        "summon water elemental", { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 26},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_summon_water_elemental,    TAR_IGNORE,     POS_STANDING,
        NULL,           SLOT(575),      50,     12,
        "",     "You feel up to calling another water elemental.", ""
    },

    {
        "tsunami",       { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 34},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_tsunami,    TAR_IGNORE,     POS_FIGHTING,
        NULL,           SLOT(576),      30,     12,
        "raging tidal wave",     "!Tsunami!", ""
    },

    {
        "earthmaw",     { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_earthmaw,    TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(577),      30,     12,
        "earthmaw",     "!earthmaw!", ""
    },

    {
        "windwall",     { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 22},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_windwall,    TAR_IGNORE,     POS_FIGHTING,
        NULL,           SLOT(578),      20,     12,
        "air blast",     "You rub the dirt and dust from your eyes.", ""
    },

    {
        "drain", { 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        spell_drain,    TAR_OBJ_INV,     POS_STANDING,
        NULL,           SLOT(579),      30,     12,
        "",     "!Drain!", ""
    },

    {
        "disenchant armor", { 53, 53, 53, 53, 53, 53, 53, 16, 53, 53, 53},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        spell_disenchant_armor,    TAR_OBJ_INV,     POS_STANDING,
        NULL,           SLOT(580),      50,     36,
        "",     "!Disenchant!", ""
    },

    {
        "disenchant weapon", { 53, 53, 53, 53, 53, 53, 53, 21, 53, 53,53},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        spell_disenchant_weapon,    TAR_OBJ_INV,     POS_STANDING,
        NULL,           SLOT(581),      50,     36,
        "",     "!Disenchant!", ""
    },
/*
    {
        "noxious gas",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_noxious_gas,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(582),      20,     12,
        "deadly poison",        "You feel able to breath properly once again.", ""
    },

    {
        "thunderclap",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 29},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 1},
        spell_thunderclap,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(583),      50,     12,
        "thunderclap",          "The ringing in your ears dies away.",""
    },
    
    {
        "inferno",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 1},
        spell_inferno,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(584),      70,     12,
        "column of fire",          "",""
    },
*/
    {
        "cone of cold",  { 53, 53, 53, 53, 53, 53, 53, 32, 53, 53, 53},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0, 0},
        spell_cone_of_cold,      TAR_IGNORE,     POS_FIGHTING,
        NULL,                   SLOT(585),      30,     12,
        "blast of cold",          "",""
    },

    {
        "protective shield",  { 53, 53, 16, 53, 53, 53, 53, 17, 53, 18, 18},
        { 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1},
        spell_protective_shield,      TAR_CHAR_SELF,     POS_STANDING,
        &gsn_protective_shield,                   SLOT(571),      75,     12,
        "",          "Your protective shield fades away.",""
    },

    {
        "timestop",  { 53, 53, 53, 53, 53, 53, 53, 40, 53, 53, 53},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0, 0},
        spell_timestop,      TAR_IGNORE,     POS_STANDING,
        &gsn_timestop,                   SLOT(586),     100,     48,
        "",          "Everything around you starts moving fast again.",""
    },
    {
        "timestop spell",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        { 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0, 0},
        spell_timestop,      TAR_IGNORE,     POS_STANDING,
        &gsn_timestop_done,                   SLOT(0),     100,     48,
        "",          "You are once again able to interfere with time.",""
    },
    
/*
    {
        "icestorm",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 32},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 1},
        spell_icestorm,      TAR_IGNORE,     POS_FIGHTING,
        NULL,                   SLOT(587),      40,     12,
        "sheets of ice",          "",""
    },

    {
        "raise hell",  { 53, 53, 53, 53, 40, 53, 53, 53, 53, 53, 53},
        { 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0, 0},
        spell_raise_hell,      TAR_CHAR_OFFENSIVE,     POS_STANDING,
        NULL,                   SLOT(588),      80,     24,
        "unholy hellspawn",          "",""
    },
*/
    {
        "consecrate",  { 53, 53, 53, 38, 53, 53, 53, 53, 53, 53, 53},
        { 0, 0, 0, 1, 0, 0, 0, 0, 0 ,0, 0},
        spell_consecrate,      TAR_IGNORE,     POS_STANDING,
        NULL,                   SLOT(589),      90,     60,
        "",          "You feel holy enough to consecrate more ground.",""
    },

    {
        "wither",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 32, 53},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        spell_wither,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(590),      40,     12,
        "withering",          "Your body feels less emancianted.",""
    },

    {
        "shadowstrike",  { 53, 53, 53, 53, 53, 53, 53, 53, 36, 53, 53},
        { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        spell_shadowstrike,      TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_shadowstrike,                   SLOT(563),      100,     12,
        "",          "The shadowy arms grasping you fade away in a smokey haze.",""
    },

    {
	"dark dream",	{ 53, 53, 53, 53, 53, 53, 53, 53, 15,53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_SLEEPING,
	&gsn_dark_dream,	SLOT( 0),	0,	0,
	"",	"",	""
    },

    {
	"shadowself",	{53, 53, 53, 53, 53, 53, 53, 53, 22, 29, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_shadowself,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(619),	20,	12,
	"",	"",	""
    },

    {
        "nightwalk",  { 53, 53, 53, 53, 53, 53, 53, 53, 28, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_nightwalk,      TAR_IGNORE,     POS_STANDING,
        NULL,                   SLOT(561),      75,     12,
	"", "", ""
    },

    {
        "lightshield",           { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightshield,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(813),	20,	18,
	"",			"The glow of purity surrounding you fades.", ""
    },

    {
	"sunbolt",	{20,20,20,20,20,20,20,20,20,20,20},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_sunbolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,	SLOT(751),	30,	12,
	"sunbolt",	"",	""
	},

    {
	"fire and ice", {53,53,53,53,53,53,53,53,53,53,24},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_fire_and_ice, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	NULL, SLOT(810), 16, 12,
	"fire and ice",	"",	""
    },
    {
	"utter heal", {53,53,34,53,53,53,53,53,53,53,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_utter_heal, TAR_CHAR_DEFENSIVE, POS_STANDING,
	NULL, SLOT(811),	75, 12,
	"", "", ""
    },
 
    { 
	"atrophy", {53,53,38,53,53,53,53,53,53,53,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_atrophy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
	&gsn_atrophy, SLOT(812),	25, 12,
	"atrophy",	"",	""
    },

/*

    {
        "slip",  { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 12},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_slip,      TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(593),      15,     12,
        "",          "",""
    },

*/

/*
 * Dragon breath
 */
    {
        "acid breath",          { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(200),	100,	24,
	"blast of acid",	"!Acid Breath!",	""
    },

    {
        "fire breath",          { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(201),	200,	24,
	"blast of flame",	"The smoke leaves your eyes.",	""
    },

    {
        "frost breath",         { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(202),	125,	24,
	"blast of frost",	"!Frost Breath!",	""
    },

    {
        "gas breath",           { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	175,	24,
	"blast of gas",		"!Gas Breath!",		""
    },

    {
        "lightning breath",     { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(204),	150,	24,
	"blast of lightning",	"!Lightning Breath!",	""
    },


/* combat and weapons skills */


    {
        "axe",                  { 1, 53, 53, 1, 1, 1, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
        "",                     "!Axe!",		""
    },

    {
        "dagger",               { 1, 1, 53, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
        "",                     "!Dagger!",		""
    },
 
    {
        "flail",                { 1, 53, 1, 1, 1, 53, 53, 1, 53, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
        "",                     "!Flail!",		""
    },
        {
        "flail arms",   {61,61,61,61,61,61,61,61,61,61,61},
        {0,0,0,0,0,0,0,0,0,0,0},
        spell_null,     TAR_IGNORE,     POS_DEAD,
        &gsn_flail_arms,        SLOT(0),	0,      0,
        "flailing bone arms",   "",     ""
        },

    {
	"mind sear",	{61,61,61,61,61,61,61,61,61,61,61},
	{1,1,1,1,1,1,1,1,1,1,1},		
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	SLOT(0),	0,	0,
	"mind sear",	"The burning in your mind fades away.",	""
    },

    {
        "mace",                 { 1, 1, 1, 1, 1, 53, 1, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
        "",                     "!Mace!",		""
    },

    {
        "polearm",              { 1, 53, 53, 1, 1, 1, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
        "",                     "!Polearm!",		""
    },
    
    {
        "shield block",         { 10, 14, 18, 11, 11, 12, 53, 53, 15, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		""
    },
 
    {
        "spear",                { 1, 1, 53, 1, 1, 1, 53, 53, 1, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      0,
        "",                     "!Spear!",		""
    },

    {
	"dual wield",	{ 12,25,53,18,18,16,28,53,17,53,53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,	POS_FIGHTING,
	&gsn_dual_wield,	SLOT( 0),	0,	0,
	"", "", ""
    },

    {
        "sword",                { 1, 1, 53, 1, 1, 1, 1, 53, 1, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
        "",                     "!sword!",		""
    },

    {
        "whip",                 { 1, 1, 1, 53, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
        "",                     "!Whip!",	""
    },

    {
	"ranger staff",	{53, 53, 53, 53, 53, 30, 53, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_ranger_staff,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	SLOT(620),	75,	12,
	"",	"",	""
    },

    {
	"counter",	{ 25, 53, 53, 27, 53, 53, 53, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_counter,	SLOT( 0),	0,	0,
	"counter strike",	"",	""
    },

    {
        "tame", {53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_tame,      SLOT(0),        0,      0,
        "",     ""
    },

    {
        "find water",   {53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_find_water,        SLOT(0),        0,      0,
        "",      ""
     },

     {
        "shield cleave",        {18, 53, 53, 22, 22, 26, 53, 53, 23, 53, 53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_shield_cleave, SLOT(0),    0,      0,
        "",     ""
     },

     {
        "darkshield",    {53, 53, 53, 53, 53, 53, 53, 53, 23, 11, 53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1 },
        spell_dark_shield,     TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,   SLOT(730),      20,     12,
        "",     "The dark shield surrounding you fades away."
    },

    {
        "spellcraft",   {53, 53, 53, 53, 53, 53, 53, 1, 53, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_SLEEPING,
        &gsn_spellcraft,        SLOT(0),        0,      0,
        "",     ""
    },



    {
        "power word fear",  {53,53,53,53,53,53,53,53,32,53,53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_power_word_fear,  TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,   SLOT(801),  20, 6,
        "", "You feel the fear plaguing your mind fade away.",
        ""
    },

    {
        "nightfall",    {53,53,53,53,53,53,53,53,38,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_nightfall,    TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,   SLOT(802),  75, 0,
        "", "", ""
    },

    {
        "decay corpse", {53,53,53,53,53,53,53,53,53,15,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_decay_corpse, TAR_IGNORE, POS_STANDING,
        NULL,   SLOT(804),  15, 0,
        "", "", ""
    },

    {
        "preserve", {53,53,53,53,53,53,53,53,53,20,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_preserve, TAR_OBJ_INV,    POS_STANDING,
        NULL,   SLOT(805),  10, 0,
        "", "", ""
    },

    {
        "imbue regeneration", {53,53,38,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_regeneration, TAR_CHAR_DEFENSIVE, POS_STANDING,
        NULL,   SLOT(806),  50, 12,
        "", "Your body stops regenerating.",    ""
    },

    {
        "restoration",  {53,53,35,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_restoration,  TAR_CHAR_DEFENSIVE, POS_STANDING,
        NULL,   SLOT(807),  50, 12,
        "", "", ""
    },


    {
	"quiet movement", {53,53,53,53,53,53,53,53,53,53,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_quiet_movement, SLOT(0), 0, 0,
	"", "You stop moving quietly.", ""
    },
    {
	"iron resolve", {53,53,53,53,53,53,53,53,53,53,53},
	{1,1,1,1,1,1,1,1,1,1,1},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_iron_resolve, SLOT(0), 0, 0,
	"",	"Your mind wanders from it's resolve.", ""
    },

    {
	"ambush",		{53, 53, 53, 53, 53, 12, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_ambush,		SLOT( 0),	0,	24,
	"surprise attack",	"!Ambush!",	""
    },

    {	
	"animate skeleton",	{53,53,53,53,53,53,53,53,53,15,53},
	{1,1,1,1,1,1,1,1,1,1,1,},
	spell_animate_skeleton,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(779),	50,	12,
	"",	"You feel your power over the dead return.",	""
    },

    {
	"mummify",	{53,53,53,53,53,53,53,53,53,35,53},
	{1,1,1,1,1,1,1,1,1,1,1,},
	spell_mummify,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(780),	150,	24,
	"",	"You feel you power over the dead return.",	""
    },

    {
	"moving ambush",		{53, 53, 53, 53, 53, 35, 53, 53,
53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_moving_ambush,		SLOT( 0),	0,	0,
	"surprise attack",	"!Ambush!",	""
    },


        {
        "pugil",        {22, 53, 53, 26, 53, 24, 20, 53, 53, 53, 53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_pugil,     SLOT(0),        0,      12,
        "pugil",        "",     "",
        },

        {
        "lash", {15,13,24,53,16,19,25,17,18,25,25},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_lash,      SLOT(0),        0,      12,
        "lash", "",     "",
        },

        {
        "protection heat cold", {53,53,53,53,53,18,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_protection_heat_cold,      SLOT(0),        0,      12,
        "",     "You no longer feel protected from the elements.", ""
        },

        {
        "evaluation",   {1,1,1,1,1,1,1,53,1,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_evaluation,        SLOT(0),        0,      0,
        "",     "",     ""
        },

    {
        "enhanced damage",      { 1, 20, 29, 1, 1, 15, 18, 53, 16, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
        "",                     "!Enhanced Damage!",	""
    },


        {
        "enhanced damage II",   {34,53,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_enhanced_damage_two,       SLOT(0),        0,      0,
        "",     "",     ""
        },

        {
        "staff",        {1,53,1,1,53,1,1,53,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_staff,     SLOT(0),        0,      0,
        "",     "",                "",
        },

        {
        "tactics",      {53,53,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_tactics,   SLOT(0),        0,      0,
        "",     "",     "",
        },

        {
        "rally",        {53,53,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_rally,     SLOT(0),        0,      0,
        "",     "Your confidence to incite others has returned.",       ""
        },

        
    {
        "backstab",             { 53, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     24,
        "backstab",             "!Backstab!",		""
    },


    {
	"strange form",	{ 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60},
	{0, 0 ,0 ,0 ,0 ,0 ,0 ,0 , 0, 0, 0},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_strange_form,	SLOT( 0),	0,	0,
	"",	"You feel your normal form returning.",	"You feel your normal form returning."
    },

    {
	"cloak", {15,15,15,15,15,15,15,15,15,15,15},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_cloak_form,	SLOT( 0), 	0,	0,
	"",	"The wind pulls your cloak away from your face.", ""
    },
			
    {
        "bite", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 0,0,0,0,0,0,0,0,0,0,0},
        spell_null, TAR_IGNORE, POS_FIGHTING,
        &gsn_chimera_lion,  SLOT( 0),   0,  0,
        "crushing bite",    "", ""
    },

    {
        "ram", {1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1},
        { 0,0,0,0,0,0,0,0,0,0,0},
        spell_null, TAR_IGNORE, POS_FIGHTING,
        &gsn_chimera_goat,  SLOT( 0),   0,  0,
        "ramming",  "", ""
    },


    {
	"tail",	{15,15,15,15,15,15,15,15,15,15,15},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_tail,	SLOT( 0),	0,	12,
	"tail attack",	"", ""
    },

    {
	"throw", 	{53, 53, 53, 53, 53, 53, 15, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_throw,	SLOT( 0),	0,	12,
	"throw",	"",	""
    },

    {
	"endure", 	{53, 53, 53, 53, 53, 53, 11, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_endure,	SLOT(0),	0,	12,
	"",	"You feel more susceptible to magic.",	""
    },
    {
	"palm", 	{53, 53, 53, 53, 53, 53, 32, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_palm,	SLOT(0),	0,	12,
	"palm",	"You stop concentrating on fighting open handed.",	""
    },

    {
	"poison dust",	{53, 53, 53, 53, 53, 53,14, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_poison_dust,	SLOT( 0),	0,	0,
	"poison",	"The poison in your blood is neutralised.", ""
    },

    {
	"blindness dust",	{53, 53, 53, 53, 53, 53,23, 53, 53, 53,53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_blindness_dust,	SLOT( 0),	0,	0,
	"",	"You rub the dust in your eyes away.", ""
    },

    {
	"roll",	{53, 53, 53, 53, 53, 53, 24, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_roll,	SLOT( 0),	0,	0,
	"",	"",	""
    },

    {
	"block",	{53, 53, 53, 53, 53, 53, 16, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_block,	SLOT( 0),	0,	0,
	"", "", ""
    },
/*
    {
        "strangle timer", {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,  TAR_IGNORE, POS_STANDING,
        &gsn_strangle_timer,    SLOT(0),0,  0,
        "", "You stop guarding your neck so carefully.",    ""
    },
*/
    {
	"strangle",	{53, 53,53, 53, 53, 53, 25, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1,1,1, 1, 1, 1},
	spell_null,	TAR_IGNORE, POS_STANDING,
	&gsn_strangle,	SLOT( 0),	0,	0,
	"strangulation attempt",	"Your neck feels better.", ""
    },


    {
	"nerve", 	{53, 53, 53, 53, 53, 53, 13, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_nerve,	SLOT( 0),	0,	0,
	"nerve",	"Your arm loses the numbness within it.",	""
    },

    {
	"follow through", 	{53, 53, 53, 53, 53, 53, 26, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_follow_through,	SLOT( 0),	0,	0,
	"follow kick",	"",	""
    },



    {
        "bash",                 { 12, 53, 53, 53, 13, 53, 53, 53, 53, 53, 
53 },
        { 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      24,
        "bash",                 "!Bash!",		""
    },

    {
        "berserk",              { 18, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        "",                     "You feel your pulse slow down.",	""
    },

    {
	"crush",	{ 16, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_crush,	SLOT( 0),	0,	12,
	"body crush",	"",	""
    },

    {
	"ground control",	{ 29,53, 53, 53, 53, 53, 27, 53, 53, 53,
53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_ground_control,	SLOT( 0),	0,	0,
	"ground control",	"",	""
    },

    {
        "circle stab",                 { 53, 14, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_circle,            	SLOT( 0),       0,      18,
        "circle stab",                 "!Circlestab!",		""
    },

   
    {
	"spellbane", { 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_spellbane,	SLOT( 0),	0,	0,
	"spellbane",	"Your aura of hatred recedes and you no longer reflect spells.",	""
    },

    {
	"warcry",	{16, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_warcry,	SLOT(0),	0, 0,
	"",	"The effects of your warcry die out.",	""
    },
    
    {
	"battlecry",		{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_battlecry,		SLOT( 0),		0,	0,
	"",			"Your battle fury dies down.",		""
    },


    {
        "dirt kicking",         { 4, 3, 53, 53, 4, 5, 4, 53, 4, 53, 53 },
        { 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	24,
	"kicked dirt",		"You rub the dirt out of your eyes.",	""
    },

    {
        "disarm",               { 15, 17, 53, 16, 16, 17, 53, 53, 16, 53, 53 },
        { 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     24,
        "",                     "!Disarm!",		""
    },
 
    {
        "dodge",                { 13, 1, 53, 53, 53, 14, 1, 5, 1, 5, 5 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
        "",                     "!Dodge!",		""
    },
 
    {
	"dual backstab",	{ 53, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dual_backstab,	SLOT( 0),	0,	0,
	"second backstab",	"!DualBackstab!",	""
    },

    {
        "envenom",              { 53, 10, 53, 53, 53, 53, 53, 53, 21, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	spell_null,		TAR_IGNORE,	  	POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	36,
	"",			"!Envenom!",		""
    },

    {
        "hand to hand",         { 5, 7, 11, 6, 6, 9, 3, 15, 9, 12, 5 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	""
    },

    {
        "kick",                 { 8, 14, 12, 10, 10, 11, 6, 53, 13, 18, 9},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
        "kick",                 "!Kick!",		""
    },

    {
        "parry",                { 1, 11, 7, 1, 1, 1, 20, 53, 16, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
        "",                     "!Parry!",		""
    },

    {
        "rescue",               { 1, 53, 53, 5, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
        "",                     "!Rescue!",		""
    },

    {
	"downstrike",	{53, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_downstrike,	SLOT(0),	0,	0,
	"downstrike",	"",	""
    },

    {
        "trip",                 { 9, 5, 53, 53, 13, 11, 10, 53, 11, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	24,
	"trip",			"!Trip!",		""
    },

    {
        "second attack",        { 5, 12, 24, 7, 7, 8, 7, 28, 6, 21, 26},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
        "",                     "!Second Attack!",	""
    },

    {
        "third attack",         { 19, 25, 53, 24, 24, 25, 25, 53, 23, 53, 
53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
        "",                     "!Third Attack!",	""
    },

    {
        "fourth attack",         { 30, 53, 53, 53, 53, 53, 30, 53, 53, 53,
53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fourth_attack,      SLOT( 0),        0,     0,
        "",                     "!Fourth Attack!",	""
    },

/* non-combat skills */

    {
	"acute vision",		{ 53, 53, 53, 53, 53, 11, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_acute_vision,	SLOT( 0),	0,	12,
	"",			"Your visions weakens",	""

    },

    {
	"barkskin",		{ 53, 53, 53, 53, 53, 18, 53 ,53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_barkskin,		SLOT( 0),	0,	12,
	"",	"The bark on your skin peels off and falls aways.", ""
    },

    {
	"enlist",	{35, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_enlist,	SLOT( 0),	0,	0,
	"",	"You decide to follow your own destiny and leave your company.", ""
    },

    {
	"animal call",	{53, 53, 53, 53, 53,20,53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_animal_call,	SLOT( 0),	0,	36,
	"", "You feel up to calling more animals.", ""
    },

    {
	"bear call",	{53, 53, 53, 53, 53,53,53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_bear_call,	SLOT( 0),	0,	12,
	"", "You feel up to calling more bears.", ""
    },
  
    {
	"camouflage",		{ 53, 53, 53, 53, 53, 8, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_camouflage,	SLOT( 0),	0,	12,
	"",			"!Camouflage!",		""
    },

    {  
	"camp",		{ 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_camp,		SLOT( 0),	0,	48,
	"",			"!Camp!",		""
    },

    { 
        "fast healing",         { 7, 12, 9, 14, 14, 15, 8, 17, 13, 14,8},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	""
    },

    {
        "haggle",               { 14, 8, 11, 15, 15, 14, 13, 13, 9, 10},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		""
    },

    {
        "hide",                 { 53, 1, 53, 53, 53, 53, 10, 53, 5, 53,
53},
	{1, 1, 1, 1, 1, 1, 1,1,1,1,1,},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	12,
	"",			"!Hide!",		""
    },

    {
	"laying hands",		{ 53, 53, 53, 35, 53, 53, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_laying_hands,	SLOT( 0),	35, 	36,
	"",			"You feel able to heal with your touch again.",	""
    },

    {
        "lore",                 { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_lore,		SLOT( 0),	0,	36,
	"",			"!Lore!",		""
    },

    {
        "meditation",           { 16, 17, 6, 8, 8, 13, 2, 7, 12, 11, 7},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		""
    },
 
    {
	"trance",	{53,53,15,53,53,53,14,15,53,14,14},
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,	TAR_IGNORE,	POS_SLEEPING,
	&gsn_trance,	SLOT( 0),	0,	0,
	"",	"",	""
    },
/*
    {
	"morph dragon",		{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,53},
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_morph_dragon,	SLOT( 0),	100,	56,
	"",			"You return to your natural form.", ""
    },
*/
    {
	"morph black dragon",		{ 53, 53, 53, 53, 53, 53, 53, 53,53, 53, 53},
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_morph_black,	SLOT( 0),	100,	56,
	"",			"You return to your natural form.", ""
    },
    {
	"morph white dragon",		{ 53, 53, 53, 53, 53, 53, 53, 53,53, 53, 53},
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_morph_white,	SLOT( 0),	100,	56,
	"",			"You return to your natural form.", ""
    },
    {
	"morph blue dragon",		{ 53, 53, 53, 53, 53, 53, 53, 53,53, 53, 53},
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_morph_blue,	SLOT( 0),	100,	56,
	"",			"You return to your natural form.", ""
    },
    {
	"morph green dragon",		{ 53, 53, 53, 53, 53, 53, 53, 53,53, 53, 53},
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_morph_green,	SLOT( 0),	100,	56,
	"",			"You return to your natural form.", ""
    },
    {
	"morph red dragon",		{ 53, 53, 53, 53, 53, 53, 53, 53,53, 53, 53},
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_morph_red,	SLOT( 0),	100,	56,
	"",			"You return to your natural form.", ""
    },

    {
	"breath dragon",	{ 53, 53, 53, 53, 53, 53, 53, 53 , 53, 53,53 },
	{ 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0, 0},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_breath_morph,			SLOT( 0),	20,	12,
	"",			"!Breathe!",	""
    },

    {
        "lunge",        { 14, 53, 53, 18, 24, 53, 53, 53, 53, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_FIGHTING,
        &gsn_lunge,      SLOT( 0),       0,      24,
        "lunge",        "",""
    },

    {
        "black guard", {61,61,61,61,61,61,61,61,61,61,61},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_SLEEPING,
        &gsn_blackjack_timer,   SLOT(0),	0,  0,
        "", "You stop guarding your head so carefully.",    ""
    },
    {
        "blackjack",    { 53, 21, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_blackjack,  SLOT( 0),       0,      24,
        "blackjack",    "You come to with a pounding headache.", ""
    },

    {
	"counter parry",	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_counter_parry,	SLOT( 0),	0,	0,
	"counter parry",	"",	""
    },

    {
        "vigilance",    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_vigilance,  SLOT( 0),       40,     12,
        "", "Your vigilance relaxes.", ""
    },

    {
        "guard call",   { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_guard_call,       TAR_IGNORE,     POS_STANDING,
        NULL,   SLOT(611),      35,     12,
        "", "You can commandeer a guard contingent once again.", ""
    },

    {
        "peek",                 { 53, 6, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		""
    },

    {
        "pick lock",            { 53, 11, 53, 53, 53, 53, 53, 53, 13, 53, 53},
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		""
    },

    {
	"skin",			{ 53, 53, 53, 53, 53, 22, 53, 53, 53, 53, 53},
	{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_skin,		SLOT( 0),	0,	24,
	"",			"You feel up to skinning and fashioning another sack.", ""
    },

    {
	"butcher",	{ 53, 53, 53, 53, 53, 14, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_butcher,	SLOT( 0),	0,	12,
	"",	"",	""
    },


    {
        "sneak",                { 53, 4, 53, 53, 53, 53, 5, 53, 6, 53,53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"You no longer feel stealthy.",	""
    },

    {
        "steal",                { 53, 6, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	24,
	"",			"!Steal!",		""
    },

    {
	"steel nerves",		{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_steel_nerves,	SLOT( 0),	0,	12,
	"",			"Your nerves beging to settle down and you don't feel as invincible.",
	""
    },

    {
        "scrolls",              { 53, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	24,
	"",			"!Scrolls!",		""
    },

    {
        "staves",               { 53, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		""
    },
    
    {
        "wands",                { 53, 53, 1, 53, 53, 53, 1, 1, 53, 1, 1 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		""
    },

    {
        "recall",               { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	12,
	"",			"You are able to recall again.",
""
    },

    {
	"channel",	{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_channel,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(555),	100,	24,	
	"",	"Your mind loses its mental strength and you feel less healthy.", ""
    },

    {
        "life transfer", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_life_transfer, TAR_CHAR_DEFENSIVE,        POS_STANDING,
        NULL,   SLOT(609),      10,     12,
        "",     "",""
    },

    {
	"safety", {20,20,20,20,20,20,20,20,20,20,20},
	{ 1, 1, 1, 1, 1 ,1,1,1, 1, 1, 1},
	spell_safety,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	SLOT(690), 75, 24,
	"",	"",	""
    },

    {
	"lifeline",	{18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_life_line,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(630),	50,	12,
	"",	"You feel the strength of your life line diminsh.", ""
    },

    {
	"badge",	{15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_badge,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(640),	75,	12,
	"",	"",	""
    },

    {
	"protectorate plate", {20, 20, 20, 20, 20, 20, 20, 20, 20, 20,20},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_protectorate_plate,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(631),	75,	12,
	"",	"",	""
    },

    {
	"decoy",	{15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_decoy,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(632),	40,	12,
	"",	"You are informed that more decoys are ready for your use.", ""
    },

    {
	"alarm",	{53, 53, 53, 53, 53, 53, 53, 30, 53, 53,53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_alarm,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(634),	50,	12,
	"",	"",	""
    },


    {
	"holy fire",	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_holy_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,	SLOT(636),	20,	12,
	"holy fire",	"",	""
    },

    {
	"blade barrier",	{53, 53, 30, 53, 53, 53, 53, 53, 53,53,53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1},
	spell_blade_barrier,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,	SLOT(637),	20,	12,
	"blade barrier",	"",	""
    },
	
    {
        "prevent", {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_prevent,  TAR_CHAR_SELF,  POS_STANDING,
        NULL,   SLOT(610),      100,    24,
        "", "You feel less healthy as your mind starts to wander from it's meditation.",
        ""
    },
    {
        "prevent healing",  {53,53,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_prevent_healing, TAR_CHAR_OFFENSIVE,  POS_FIGHTING,
        &gsn_prevent_healing,   SLOT(803),  20, 0,
        "", "You feel your health returning.",  ""
    },


    {
        "aura of sustenance", {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_aura_of_sustenance,    SLOT( 0),    50,     36,
        "", "Your body begins to feel the bite of hunger once again.", ""
    },

    {
        "spiritblade", {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
        spell_spiritblade,      TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,   SLOT(597), 30,     12,
        "spiritblade",  "", ""
    },

    {
        "iceball",      {53, 53, 53, 53, 29, 53, 53, 53, 53, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_iceball,  TAR_IGNORE,     POS_FIGHTING,
        NULL,   SLOT(616),      20,     12,
        "iceball",      "", ""
    },

    {
        "hand of vengeance", { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_hand_of_vengeance, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,   SLOT(613),      25,     24,
        "hand of vengance",     "", ""
    },

    {
        "wanted", { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null, TAR_IGNORE, POS_SLEEPING,
        &gsn_wanted,     SLOT( 0),       0,      0,
        "",     "",     ""
    },

    {
        "bandage", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_bandage,    SLOT( 0),       0,     36,
        "",     "You are once more able to administer battle aid.", ""
    },

    {
	"trophy",	{21,21,21,21,21,21,21,21,21,21,21},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_RESTING,
	&gsn_trophy,	SLOT( 0),	0,	0,
	"",	"You feel up to making a new trophy.",	""
    },

    {
	"shadowplane",	{60,60,60,60,60,60,60,60,60,60,60},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	spell_null,	TAR_IGNORE,	POS_SLEEPING,
	&gsn_shadowplane,	SLOT(0),	0, 0,
	"prescence in the shadowplane",	"",	""
    },

    {
	"request",	{53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,	
	&gsn_request,	SLOT(0),	0,	0,
	"",	"Your contemplation of the good you have seen is over.",
	""
    },

    {
	"battleshield", {20,20,20,20,20,20,20,20,20,20,20},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_battleshield,	SLOT( 0),	0,	0,
	"",	"You feel up to forging a new shield.",	""
    },


    {
        "herb", {53, 53, 53, 53, 53, 10, 53, 53, 53, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_herb,       SLOT( 0),       0,      12,
        "", "The herbs look more plentiful.", ""
    },

    {
        "cleave",      {53, 53, 53, 53, 30, 53, 53, 53, 53, 53, 53},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,  TAR_IGNORE,     POS_STANDING,
        &gsn_cleave,   SLOT(616),      0,     12,
        "cleave",      "", ""
    },

    {
	"revolt",	{25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_revolt,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_revolt,	SLOT(623),	40,	12,
	"",	"",	""
    },

    {
	"riot",	{15, 15, 15, 15, 15, 15, 15, 15, 15, 15,15},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_riot,	SLOT( 0),	0,	12,
	"",	"Your furious rioting emotions subside.", ""
    },

    {
	"vanish",	{18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	&gsn_vanish,	SLOT( 0),	0,	12,
	"",	"You feel ready to vanish once again.",	""
    },

    {
	"familiar",	{ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_familiar,	TAR_IGNORE, POS_FIGHTING,
	NULL,	SLOT(595),	75,	12,
	"",	"You feel up to summoning a new familiar.", ""
    },


    {
        "shadowgate", {25,25,25,25,25,25,25,25,25,25,25},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_shadowgate, SLOT( 0),   75, 36,
        "", "",""
    },

    {
        "eye of the predator", {20,20,20,20,20,20,20,20,20,20,20},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null,     TAR_IGNORE, POS_STANDING,
        &gsn_eye_of_the_predator,    SLOT( 0),   50, 12,
        "", "Your predatory awareness receedes.", ""
    },


    {
        "frostbolt",    {53,53,53,53,53,53,53,53,53,53,22},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_frostbolt,    TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL, SLOT(780),  25, 12,
        "frostbolt",    "", "",
    },

    {
        "icelance", {53,53,53,53,53,53,53,23,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_icelance, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        NULL,  SLOT(781),  24, 12,
        "icelance", "", "",
    },

    {
        "earthfade",    {53,53,53,53,53,53,53,53,53,53,39},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_earthfade,    TAR_CHAR_SELF,  POS_STANDING,
        &gsn_earthfade, SLOT(782),  100,    12,
        "", "Your slowly emerge from the earth.", ""
    },

    {
        "forget",   {53,53,53,53,53,53,53,53,53,29,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_forget,   TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_forget,    SLOT(783),  50, 12,
        "", "Your mind becomes clearer once again.",    ""
    },

    {
        "earthbind",    {53,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_earthbind,    TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_earthbind, SLOT(784),  20, 12,
        "", "Your feet no longer feel bound to the ground.",    ""
    },

    {
        "cremate",  {53,53,53,53,53,53,53,53,53,26,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_cremate,  TAR_CHAR_OFFENSIVE,    POS_FIGHTING,
        NULL,   SLOT(785),  20, 12,
        "blast of fire",   "",  ""
    },

    {
        "divine touch", {53,53,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_divine_touch,     TAR_CHAR_SELF,  POS_STANDING,
        &gsn_divine_touch,   SLOT(786), 30, 12,
        "", "Your hands lose their divine touch.",  ""
    },

    {
        "transfer object",  {53,53,53,53,53,53,53,32,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_transfer_object,  TAR_IGNORE,    POS_STANDING,
        NULL,   SLOT(787),  30, 12,
        "", "", ""
    },

    {
        "disintergrate",    {53,53,53,53,53,53,53,40,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_disintergrate,    TAR_CHAR_OFFENSIVE, POS_STANDING,
        NULL,   SLOT(788),  100,    12,
        "disintegration",   "", ""
    },

    {
        "grounding",    {53,53,53,53,53,53,53,53,53,53,29},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_grounding,    TAR_CHAR_SELF,  POS_STANDING,
        &gsn_grounding, SLOT(789),  70, 12,
        "", "You lose your grounding against electricty.",  ""
    },

    {
        "shock sphere", {53,53,53,53,53,53,53,18,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_shock_sphere, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
        &gsn_shock_sphere,  SLOT(790),  20, 12,
        "shock sphere", "The ringing in your ears dies away.", ""
    },

    {
        "forage",   {53,53,53,53,53,25,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_forage,    SLOT(0),    0,  0,
        "", "", ""
    },

    {
        "assassinate",   {53,53,53,53,53,53,37,53,35,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_assassinate,    SLOT(0),    0,  0,
        "assassination attempt", "", ""
    },

    {
        "defend",   {29,53,53,24,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_defend,    SLOT(0),    0,  0,
        "", "", ""
    },

    {
        "intimidate",   {53,53,53,53,36,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_intimidate,    SLOT(0),    0,  0,
        "", "", ""
    },

    {
        "escape",   {53,27,53,53,53,53,53,53,53,53,53},
        {1,1,1,1,1,1,1,1,1,1,1},
        spell_null, TAR_IGNORE, POS_STANDING,
        &gsn_escape,    SLOT(0),    0,  0,
        "", "", ""
    },


    {
	"deathstrike",	{ 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_deathstrike,	SLOT( 0),	0,	0,
	"deathstrike",	"",	""
    },

    {
        "empower",           { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_empower,           SLOT( 0),       0,      24,
        "",                     "You feel able to reforge dark matter once again.",       ""
    }

};

const   struct  group_type      group_table     [MAX_GROUP]     =
{

    {
        "class basics",       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "sword", "axe", "dagger", "flail", "mace", "polearm",
        "shield block", "spear", "whip", "bash", "berserk", "dirt kicking",
        "disarm", "dodge", "enhanced damage", "hand to hand", "kick",
        "parry", "rescue", "trip", "second attack", "third attack",
  	"firestream", "fireshield", "iceshield", "dark dream", "embalm",
	"shadowself", "bless weapon", "barkskin","alarm","nightwalk",
	"blade barrier", "holy fire", "warcry", "lash", "evaluation",
	"circle stab", "turn undead", "concatenate", "camouflage", "ambush",
	"frostbolt",	"icelance",	"earthfade", "forget", "earthbind",
	"cremate", "divine touch", "transfer object", "disintergrate",
	"grounding", "shock sphere", "forage", "assassinate",
	"staff", "pugil", "enhanced dmaage II", "animate skeleton",
		"mummify", "protection heat cold", "transfer object",
	"defend", "intimidate", "escape", "moving ambush", "soulbind",
	"absorb","wrath", "dark wrath", "acute vision", "laying hands",
	"dual backstab", "power word kill", "power word stun", "evil eye",
	"web", "animate dead", "skin", "camp", "fourth attack",
	"animate object", "demand", "lifebane", "deathspell",
	"morph white dragon", "morph black dragon", "morph blue dragon",
	"morph green dragon", "morph red dragon",
	"breath dragon", "summon fire elemental", "summon water elemental",
	"crush", "ground control", "palm","endure", "follow through",
	"throw", "nerve", "poison dust", "blindness dust", "block",
	"roll",	"strangle", 
	"summon air elemental", "summon earth elemental", "drain", "tsunami",
	"earthmaw", "disenchant armor", "disenchant weapon","dual wield",
 	"windwall", "butcher","shadowself", "bark skin", "utter heal",
	"cone of cold", "timestop", "icestorm", "raise hell",  "consecrate",
    	"wither", "moonshine", "shadowstrike", "nightmare", "slip", 
	"protective shield", "animal call", "lunge", "blackjack",
	"trance", "herb","cleave","wither","iceball",
	"lesser golem","ranger staff", "greater golem",
	"preserve", "power word fear", "nightfall", "prevent healing",
	"decay corpse", "imbue regeneration", "restoration",
	"undead drain", "atrophy", "fire and ice",
	"counter","bear call", "enlist", "downstrike", "spellcraft",
	"tame", "darkshield", "find water", "shield cleave",
	"curse weapon", "talk to dead", "focus",

	"recharge", 
        "fast healing", "haggle", "meditation", "recall", "backstab",
	"envenom", "camouflage", "hide", "peek", "pick lock", "sneak", "steal", "scrolls",
 	"acid blast", "armor", "bless", "blindness",
	"burning hands", "call lightning", "calm", "cancellation", "colour spray",
	"continual light", "control weather", "create food", "create rose",
	"create spring", "create water", "cure blindness", "cure critical",
	"cure disease", "cure light", "cure poison", "cure serious", "curse",
	"demonfire", "detect evil", "detect good", "detect hidden",
	"detect invis", "detect magic", "detect poison", "dispel evil",
	"dispel good", "dispel magic", "earthquake", "enchant armor",
	"enchant weapon", "energy drain", "faerie fire", "faerie fog",
	"farsight", "fireball", "fireproof", "flamestrike", "fly", "frenzy",
	"gate", "giant strength", "harm", "haste", "heal", "heat metal",
	"holy word", "identify", "infravision", "invisibility", "know alignment",
	"lightning bolt", "locate object", "magic missile", "mass healing",
	"mass invis", "pass door", "plague", "poison", "protection evil",
	"protection good", "ray of truth", "refresh", "remove curse",
	"sanctuary", "shield", "shocking grasp", "sleep", "slow",
	"stone skin", "summon", "teleport", "ventriloquate", "weaken",
	"word of recall", "cause critical", "cause light", "cause serious",
	"chain lightning", "change sex", "charm person", "chill touch",
	"ice pox" } 
    },

    {
        "class default",      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "" }
    },

/* You don't need to group Clan abilities here, the assignment of these
are now done in act_wiz.c's do_induct() function. These entries just make
it easier to see what abilities each clan gets. (Ceran)
*/
    {
        "ancient", {0,0,0,0,0,0,0,0,0,0,0},
        { "shadowgate", "eye of the predator", "cloak" }
    },

    {
        "life",    {0,0,0,0,0,0,0,0,0,0,0},
        { "sunbolt","aura of sustenance", "prevent", "life", 
	  "safety", "lifeline", "lightshield" }
    },

    {
        "arcana",  {0,0,0,0,0,0,0,0,0,0,0},
        { "brew", "familiar","channel", "venueport", "spiritblade" ,
	"team spirit"}
    },

    {
        "crusader",  {0,0,0,0,0,0,0,0,0,0,0},
        { "spellbane", "battlecry", "steel nerves" , "vigilance",
"bandage", "trophy"}
    },

    {
        "enforcer", {0,0,0,0,0,0,0,0,0,0,0},
        {"trace", "true sight",  "guard call", "hand of vengeance",
"wanted" }
    },

    {
        "outlaw",  {0,0,0,0,0,0,0,0,0,0,0},
        { "revolt", "decoy", "vanish" }
    },

};

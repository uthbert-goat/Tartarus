#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

char *const distance[7]=
{
" **** 1 %s **** ", " **** 2 %s **** ", " **** 3 %s **** ", 
" **** 4 %s **** ", " **** 5 %s **** ", " **** 6 %s **** ", " **** 7 %s **** "
};

void show_char_to_char_0 args((CHAR_DATA *victim, CHAR_DATA *ch));
void scan_list           args((ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void do_scan(CHAR_DATA *ch, char *argument)
{
   extern char *const dir_name[];
   char arg1[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *scan_room;
   EXIT_DATA *pExit;
   sh_int door, depth;

   argument = one_argument(argument, arg1);

        if (!str_cmp(arg1, "n") || !str_cmp(arg1, "north")) door = 0;
   else if (!str_cmp(arg1, "e") || !str_cmp(arg1, "east"))  door = 1;
   else if (!str_cmp(arg1, "s") || !str_cmp(arg1, "south")) door = 2;
   else if (!str_cmp(arg1, "w") || !str_cmp(arg1, "west"))  door = 3;
   else if (!str_cmp(arg1, "u") || !str_cmp(arg1, "up" ))   door = 4;
   else if (!str_cmp(arg1, "d") || !str_cmp(arg1, "down"))  door = 5;
   else { send_to_char("Which way do you want to scan?\n\r", ch); return; }

   act("You peer intently $T.", ch, NULL, dir_name[door], TO_CHAR);
   act("$n peers intently $T.", ch, NULL, dir_name[door], TO_ROOM);
   sprintf(buf, "Looking %s you see:\n\r", dir_name[door]);
                                                                                  
   scan_room = ch->in_room;

   for (depth = 1; depth <= (ch->level/10)+1; depth++)
   {
      if ((pExit = scan_room->exit[door]) != NULL)
      {
	if (!IS_SET(pExit->exit_info, EX_CLOSED))
	{
         scan_room = pExit->u1.to_room;
	 if (scan_room == NULL) return;
         scan_list(pExit->u1.to_room, ch, depth, door);
	 send_to_char("\n\r", ch);
	}
      }
   }
   return;
}

void scan_list(ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth,
               sh_int door)
{
   CHAR_DATA *rch;
   extern char *const dir_name[];
   extern char *const distance[];
   char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

   buf[0] = '\0';

   if (scan_room == NULL) return;
   sprintf(buf2, distance[(depth-1)], dir_name[door]);
   strcat(buf, buf2);
   send_to_char(buf, ch);
   send_to_char("\n\r", ch);
   for (rch=scan_room->people; rch != NULL; rch=rch->next_in_room)
   {
      if (rch == ch) continue;
      if (!IS_NPC(rch) && rch->invis_level > get_trust(ch)) continue;
      if (can_see(ch, rch)) show_char_to_char_0(rch, ch);
   }
   return;
}

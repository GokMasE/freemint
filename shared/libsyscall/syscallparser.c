/*
 * $Id$
 * 
 * This file belongs to FreeMiNT. It's not in the original MiNT 1.12
 * distribution. See the file CHANGES for a detailed log of changes.
 * 
 * 
 * Copyright 2000-2004 Frank Naumann <fnaumann@freemint.de>
 * All rights reserved.
 * 
 * Please send suggestions, patches or bug reports to me or
 * the MiNT mailing list.
 * 
 * 
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "syscalldefs.h"
#include "syscallparser.h"


void
print_head(FILE *out, const char *myname)
{
	time_t t = time(NULL);
	
	fprintf(out, "/*\n * %s, %s *\n", myname, ctime (&t));
	fprintf(out,
		" * DO NOT EDIT - this file is automatically generated!\n"
		" */\n"
	);
	fprintf(out, "\n");
	
	fprintf(out,
		"/*                                                                      \n"
		" * This file belongs to FreeMiNT. It's not in the original MiNT 1.12    \n"
		" * distribution. See the file CHANGES for a detailed log of changes.    \n"
		" *                                                                      \n"
		" *                                                                      \n"
		" * Copyright 2000-2004 Frank Naumann <fnaumann@freemint.de>             \n"
		" * All rights reserved.                                                 \n"
		" *                                                                      \n"
		" * Please send suggestions, patches or bug reports to me or             \n"
		" * the MiNT mailing list.                                               \n"
		" *                                                                      \n"
		" *                                                                      \n"
		" * This file is free software; you can redistribute it and/or modify    \n"
		" * it under the terms of the GNU General Public License as published by \n"
		" * the Free Software Foundation; either version 2, or (at your option)  \n"
		" * any later version.                                                   \n"
		" *                                                                      \n"
		" * This file is distributed in the hope that it will be useful,         \n"
		" * but WITHOUT ANY WARRANTY; without even the implied warranty of       \n"
		" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        \n"
		" * GNU General Public License for more details.                         \n"
		" *                                                                      \n"
		" * You should have received a copy of the GNU General Public License    \n"
		" * along with this program; if not, write to the Free Software          \n"
		" * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.            \n"
		" *                                                                      \n"
		" */                                                                     \n"
	);
	fprintf(out, "\n");
}

void
generate_args(FILE *out, struct arg *l, const char *pre, int flag, const char *post)
{
	while (l)
	{
		fprintf(out, "%s%s ", pre, l->types);
		
		if (l->flags & FLAG_POINTER)
			fprintf(out, "*");
		
		fprintf(out, "%s", l->name);
		
		if (l->flags & FLAG_ARRAY)
			fprintf(out, " [%i]", l->ar_size);
		
		if (l->next || flag)
			fprintf(out, "%s", post);
		
		l = l->next;
	}
}

int
arg_length(struct arg *l)
{
	int length = 0;
	
	while (l)
	{
		length++;
		l = l->next;
	}
	
	return length;
}

/* from parser */
int yyparse(void);
extern FILE *yyin, *yyout;

int errors = 0;

int
parse_syscall_description(FILE *infile)
{
	yyin = stdin;

	if (infile)
		yyin = infile;

	yyparse();

	return errors;
}

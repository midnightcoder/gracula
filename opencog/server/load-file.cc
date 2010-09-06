/*
 * load-file.cc
 *
 * Utility helper function -- load scheme code from a file
 * Copyright (c) 2008 Linas Vepstas <linasvepstas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_GUILE

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <opencog/guile/SchemeEval.h>

namespace opencog {

/**
 * Load scheme code from a file. 
 * The code will be loaded into a running instance of the evaluator.
 * Parsing errors will be printed to stderr.
 *
 * Return errno if file cannot be opened.
 */
int load_scm_file (const char * filename)
{
#define BUFSZ 400
	char buff[BUFSZ];

	FILE * fh = fopen (filename, "r");
	if (NULL == fh)
	{
		int norr = errno;
		fprintf(stderr, "Error: %d %s: %s\n", 
			norr, strerror(norr), filename);
		return norr;
	}

	SchemeEval &evaluator = SchemeEval::instance();
	int lineno = 0;

	while(1)
	{
		char * rc = fgets(buff, BUFSZ, fh);
		if (NULL == rc) break;
		std::string rv = evaluator.eval(buff);

		if (evaluator.eval_error())
		{
			fprintf(stderr, "File: %s line: %d\n", filename, lineno);
			fprintf(stderr, "%s\n", rv.c_str());
		}
		lineno ++;
	}
	
	fclose (fh);
	return 0;
}

}
#endif /* HAVE_GUILE */

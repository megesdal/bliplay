/**
 * Copyright (c) 2014 Simon Schoenenberger
 * http://blipkit.monoxid.net/
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "BKSTParser.h"
#include "BKCompiler2.h"

static char const * filename = "/Users/simon/Downloads/test-new-format.blip";
//static char const * filename = "/Users/simon/Downloads/base64.blip";

int main (int argc, char * argv [])
{
	BKSTTokenType token;
	BKSTParser parser;
	BKSTCmd cmd;
	BKCompiler2 compiler;

	FILE * file = fopen (filename, "r");

	if (file == NULL) {
		fprintf (stderr, "No such file: %s\n", filename);
		return 1;
	}

	BKSTParserInitWithFile (& parser, file);
	BKCompiler2Init (& compiler);

	while ((token = BKSTParserNextCommand (& parser, & cmd))) {
		printf ("%d %s %ld %d:%d\n", token, cmd.name, cmd.numArgs, cmd.lineno, cmd.colno);

		if (cmd.numArgs > 0) {
			for (BKInt i = 0; i < cmd.numArgs; i ++) {
				printf ("  '%s' (%ld)\n", cmd.args[i].arg, cmd.args[i].size);
			}
		}

		BKCompiler2PushCommand (& compiler, & cmd);
	}

	if (BKCompiler2Terminate (& compiler, 0) < 0) {
		return 1;
	}

	printf("\n\n");

	BKByteBuffer * buffer;

	for (BKInt i = 0; i < compiler.cmdGroups.length; i ++) {
		buffer = BKArrayGetItemAtIndex (& compiler.cmdGroups, i);
		printf("%u %lu\n", i, buffer -> size);
	}

	BKSTParserDispose (& parser);
	BKCompiler2Dispose (& compiler);

	fclose (file);

    return 0;
}
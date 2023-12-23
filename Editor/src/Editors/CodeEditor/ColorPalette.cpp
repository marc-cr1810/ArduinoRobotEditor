#include "epch.h"
#include "ColorPalette.h"

const TextEditor::Palette& ColorPalette::GetDraculaPalette()
{
	const static TextEditor::Palette p = { {
		0xfff2f8f8,	// Default
		0xffd69c56,	// Keyword	
		0xffF993BD,	// Number
		0xff8CFAF1,	// String
		0xff8CFAF1, // Char literal
		0xffffffff, // Punctuation
		0xffc679ff,	// Preprocessor
		0xffeeeeee, // Identifier
		0xfffde98b, // Known identifier
		0xffF993BD, // Preproc identifier
		0xffa47262, // Comment (single line)
		0xffa47262, // Comment (multi line)
		0xff362A28, // Background
		0xffe0e0e0, // Cursor
		0x805a4744, // Selection
		0x805555FF, // ErrorMarker
		0x405555FF, // Breakpoint
		0xffa47262, // Line number
		0x40000000, // Current line fill
		0x40808080, // Current line fill (inactive)
		0x40a0a0a0, // Current line edge
	} };
	return p;
}

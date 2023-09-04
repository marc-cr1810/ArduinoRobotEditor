#include "epch.h"
#include "ColorPalette.h"

const TextEditor::Palette& ColorPalette::GetDraculaPalette()
{
	const static TextEditor::Palette p = { {
		0xffF8F8F2,	// Default
		0xffd69c56,	// Keyword	
		0xff00ff00,	// Number
		0xff7070e0,	// String
		0xff70a0e0, // Char literal
		0xffffffff, // Punctuation
		0xff408080,	// Preprocessor
		0xffaaaaaa, // Identifier
		0xff9bc64d, // Known identifier
		0xffc040a0, // Preproc identifier
		0xff6272A4, // Comment (single line)
		0xff6272A4, // Comment (multi line)
		0xff282A36, // Background
		0xffe0e0e0, // Cursor
		0x8044475A, // Selection
		0x80FF5555, // ErrorMarker
		0x40f08000, // Breakpoint
		0xff707000, // Line number
		0x40000000, // Current line fill
		0x40808080, // Current line fill (inactive)
		0x40a0a0a0, // Current line edge
	} };
	return p;
}

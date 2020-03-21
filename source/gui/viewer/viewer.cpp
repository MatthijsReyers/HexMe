#include "viewer.h"

namespace gui
{
	viewer::viewer(utils::file File) : file(File)
	{
		// Create window
		this->window = newwin(50, 50, 1, 0);
		this->headerLength = utils::getHeaderLength(file.getHeader());

		// Initial drawing of window.
		this->onResize();

		// Initial refresh of window.
		this->onRefresh();
	}

	viewer::~viewer()
	{
		delwin(window);
	}

	viewer& viewer::onRefresh()
	{
		for (int r = 0; r < rows; r++)
			this->drawRow(r);
		wrefresh(window);
		return *this;
	}

	viewer& viewer::onResize()
	{
		// // Clear window.
		wclear(window);

		// Calculate new row & column count.
		this->columns = (getmaxx(stdscr) - LINENUMBERSWIDTH - 1) / COLUMNWIDTH;
		this->rows = getmaxy(stdscr) - 4;

		// Calculate new size.
		this->width = LINENUMBERSWIDTH + columns * COLUMNWIDTH + 1; 
		this->height = getmaxy(stdscr) - 2;

		// Resize window.
		wresize(window, height, width);

		// Draw window contents.
		this->drawBorders();
		for (int r = 0; r < rows; r++)
			this->drawRow(r);

		return *this;
	}

	viewer& viewer::onMoveCursor(int direction)
	{
		// Update file cursor position.
		unsigned long long cursor = file.getCursorLocation() + direction;
		if (cursor > file.getFileEnd()) return *this;
		else if (direction > 0) file.moveCursor(cursor);
		else if (file.getCursorLocation() < (unsigned long long)(-direction)) return *this;
		else file.moveCursor(cursor);

		// Check if top row needs to be moved.
		unsigned long long lastIndexDisplayed = 8 * columns * (rows + topRow) - 1;
		if (lastIndexDisplayed < cursor) topRow++;
		else if (topRow*columns*8 > cursor) topRow--;

		return *this;
	}

	int viewer::getColumnCount()
	{
		return this->columns;
	}

	int viewer::getByteColor(byte b, unsigned long long index, unsigned long long cursor)
	{
		int c = (int)((unsigned char)b);
		if (index == cursor) return 9;
		else if (index < headerLength) return 4;
		else if (c == 0x00) return 2;		// RED
		else if (c <= 0x0f) return 7;		// CYAN
		else if (c <= 0x1f) return 5;		// BLUE
		else if (c == 0xff) return 2;		// RED
		else if (c >= 0x7f) return 6;		// PURPLE
		else return 0;
	}

	void viewer::drawRow(unsigned long long r)
	{
		auto cursor = file.getCursorLocation();

		// Row number.
		unsigned long long rowIndex = ((r+topRow) * columns * 8);
		std::stringstream ss;
		ss << std::hex << rowIndex;
		std::string zeros = "00000000";
		zeros.erase(zeros.length() - ss.str().length());
		zeros.append(ss.str());

		// Draw & color row number.
		if (rowIndex == getYofCursor(cursor)*columns*8)
			wattron(window, COLOR_PAIR(9));
		mvwprintw(window, 1+r, 2, zeros.c_str());
		wattroff(window, COLOR_PAIR(9));

		ss.clear();
		ss << "  (x:" << std::dec << getXofCursor(cursor) << ", y:" << getYofCursor(cursor);
		ss << ", row:" << std::hex << getYofCursor(cursor)*(columns*8);
		ss << ", topRow:" << std::hex << topRow;
		ss << ", cursor:" << std::hex << cursor;
		ss << ", fileend:" << std::hex << file.getFileEnd() << ")  ";
		mvaddstr(0,0,ss.str().c_str());

		// Set cursor to start of row.
		file.moveCursor(rowIndex);

		if (file.getFileEnd() < rowIndex) {
			for (int c = 0; c < columns; c++) {
				for (int b = 0; b < 8; b++) {
					mvwprintw(window, 1+r, 13 + c*26 + b*3, "  ");
					mvwprintw(window, 1+r, 13 + columns*26 + c*11 + b, " ");;
				}
			}
		}

		else for (int c = 0; c < columns; c++) {
			for (int b = 0; b < 8; b++) {
				// Check if there are any bytes left in the buffer.
				if (file.getBytesAfterCursor() == 0) {
					mvwprintw(window, 1+r, 13 + c*26 + b*3, "  ");
					mvwprintw(window, 1+r, 13 + columns*26 + c*11 + b, " ");
				}
				else {
					// Get byte & increment file cursor.
					byte current = file.getCurrentByte();
					file.incCursor();

					// Get appropriate color for byte.
					int color = getByteColor(current, ((r+topRow)*columns*8)+(c*8)+b, cursor);
					wattron(window, COLOR_PAIR(color));

					// Draw hex representation of byte.
					mvwprintw(window, 1+r, 13 + c*26 + b*3, utils::byteToHexString(current).c_str());

					// Draw char representation of byte.
					if (current < 127 && current > 32) mvwaddch(window, 1+r, 13 + columns*26 + c*11 + b, current);
					else mvwprintw(window, 1+r, 13 + columns*26 + c*11 + b, ".");

					// Disable color again.
					wattroff(window, COLOR_PAIR(color));
				}
			}
		}
		
		
		for (int c = 1; c < columns; c++) {
			mvwprintw(window, 1+r, 11 + c*26, "│");
			mvwprintw(window, 1+r, 11 + columns*26 + c*11, "│");
		}

		mvwprintw(window, 1+r, 11 + columns*26, "║");
		mvwprintw(window, 1+r, 11, "║");

		file.moveCursor(cursor);
	}

	void viewer::drawBorders()
	{
		const static char* horizontal = "═";
		const static char* vertical = "║";

		const static char* cornerTL = "╔";
		const static char* cornerTR = "╗";
		const static char* cornerBL = "╚";
		const static char* cornerBR = "╝";

		const static char* tBracketD = "╦";
		const static char* tBracketU = "╩";

		const static char* tThinD = "╤";
		const static char* tThinU = "╧";

		for (int y = 0; y < height-1; y++) {
			mvwprintw(window, y, 0, vertical);
			mvwprintw(window, y, width-2, vertical);
		}

		for (int x = 0; x < width-1; x++) {
			mvwprintw(window, 0, x, horizontal);
			mvwprintw(window, height-1, x, horizontal);
		}

		mvwprintw(window, 0, 0, cornerTL);
		mvwprintw(window, 0, width-2, cornerTR);
		mvwprintw(window, height-1, 0, cornerBL);
		mvwprintw(window, height-1, width-2, cornerBR);

		mvwprintw(window, 0, 11, tBracketD);
		mvwprintw(window, height-1, 11, tBracketU);

		mvwprintw(window, 0, 11 + columns * 26, tBracketD);
		mvwprintw(window, height-1, 11 + columns * 26, tBracketU);

		for (int c = 1; c < columns; c++) {
			mvwprintw(window, 0, 11 + c * 26, tThinD);
			mvwprintw(window, height-1, 11 + c * 26, tThinU);
			mvwprintw(window, 0, 11 + columns * 26 + c * 11, tThinD);
			mvwprintw(window, height-1, 11 + columns * 26 + c * 11, tThinU);
		}
	}

	unsigned long long viewer::getYofCursor(unsigned long long cursor)
	{
		return cursor / (columns*8);
	}

	unsigned long long viewer::getXofCursor(unsigned long long cursor)
	{
		return cursor % (columns*8);
	}
}

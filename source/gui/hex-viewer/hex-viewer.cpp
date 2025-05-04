#include "hex-viewer.hpp"

#include <sstream>

namespace gui
{
	HexViewer::HexViewer(std::weak_ptr<File> f) : file(f)
	{
		this->window = newwin(50, 50, 1, 0);
	}

	HexViewer::~HexViewer()
	{
		delwin(window);
	}

	HexViewer& HexViewer::onRefresh()
	{
		auto f = this->file.lock();

		if (f->fileType.has_value()) {
			this->headerLength = f->fileType.value().second.size();
		} else {
			this->headerLength = 0;
		}

		auto cursor = f->getCursor();
		auto cursorRow = cursor / (columns*8);
		auto lastRow = f->size() / (columns*8);
		auto bottomRow = topRow + rows - 1;

		// Check: there can never be an empty row unless the window is bigger than the file.
		if (bottomRow > lastRow) {
			if (bottomRow - lastRow > topRow)
				topRow = 0;
			else topRow -= bottomRow - lastRow;
		}

		// Check: is the top row below the cursor?
		if (topRow > cursorRow)
			topRow = cursorRow;

		// Check: is the bottom row above the cursor?
		if (cursorRow > bottomRow)
			topRow = cursorRow - rows + 1;

		// Redraw all elements;
		wclear(window);
		this->drawBorders();
		for (int r = 0; r < rows; r++)
			this->drawRow(r);

		wrefresh(window);
		return *this;
	}

	HexViewer& HexViewer::onResize()
	{
		// Calculate new row & column count.
		this->columns = (getmaxx(stdscr) - LINE_NUMBERS_WIDTH - 1) / COLUMN_WIDTH;
		this->rows = getmaxy(stdscr) - 4;

		// Clamp columns if needed
		if (this->maxColumns.has_value()) {
			this->columns = std::min(this->columns, this->maxColumns.value());
		}

		// Calculate new size.
		this->width = LINE_NUMBERS_WIDTH + columns * COLUMN_WIDTH + 1; 
		this->height = getmaxy(stdscr) - 2;

		// Resize window.
		wresize(window, height, width);

		// Draw window contents.
		this->onRefresh();

		return *this;
	}

	int HexViewer::getColumnCount()
	{
		return this->columns;
	}

	int HexViewer::getByteColor(u_int8_t b, u_int64_t index, u_int64_t cursor)
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

	std::string HexViewer::byteToHexString(const u_int8_t b)
    {
        std::stringstream ss;
        int converted = (int)((unsigned char)b);
        if (converted <= 0x0f) ss << "0";
        ss << std::hex << converted;
        return ss.str();
    }

	void HexViewer::drawRow(u_int64_t r)
	{
		auto f = this->file.lock();
		auto cursor = f->getCursor();

		// Draw interior borders
		for (int c = 1; c < columns; c++) {
			mvwprintw(window, 1+r, 11 + c*26, "│");
			mvwprintw(window, 1+r, 11 + columns*26 + c*11, "│");
		}
		mvwprintw(window, 1+r, 11 + columns*26, "║");
		mvwprintw(window, 1+r, 11, "║");

		// Address of the first byte in this row.
		u_int64_t rowIndex = ((r+topRow) * columns * 8);

		// Is this row index even still inside the file?
		if (f->size() < rowIndex) {
			return;
		}

		// Draw row number
		std::stringstream ss;
		ss << std::hex << rowIndex;
		std::string rowNumber = "00000000";
		rowNumber.erase(rowNumber.length() - ss.str().length());
		rowNumber.append(ss.str());

		// Draw & color row number.
		if (rowIndex == getYofCursor(cursor)*columns*8)
			wattron(window, COLOR_PAIR(9));
		mvwprintw(window, 1+r, 2, rowNumber.c_str());
		wattroff(window, COLOR_PAIR(9));


		for (int c = 0; c < columns; c++) 
		{
			for (int b = 0; b < 8; b++) 
			{
				size_t byteIndex = ((r+topRow)*columns*8)+(c*8)+b;
				auto byte = f->getByte(byteIndex);

				// Did we get a byte or is the index out of bounds?
				if (!byte.has_value()) {
					break;
				}

				u_int8_t current = byte.value();

				// Get appropriate color for byte.
				int color = getByteColor(current, byteIndex, cursor);
				wattron(window, COLOR_PAIR(color));

				// Draw hex representation of byte.
				mvwprintw(window, 1+r, 13 + c*26 + b*3, byteToHexString(current).c_str());

				// Draw char representation of byte.
				if (current < 127 && current > 32) mvwaddch(window, 1+r, 13 + columns*26 + c*11 + b, current);
				else mvwprintw(window, 1+r, 13 + columns*26 + c*11 + b, ".");

				// Disable color again.
				wattroff(window, COLOR_PAIR(color));
			}
		}
	}

	void HexViewer::drawBorders()
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

	u_int64_t HexViewer::getYofCursor(u_int64_t cursor)
	{
		return cursor / (columns*8);
	}

	u_int64_t HexViewer::getXofCursor(u_int64_t cursor)
	{
		return cursor % (columns*8);
	}
}

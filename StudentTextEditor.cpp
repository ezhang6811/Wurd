#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <list>

#include <iostream>
#include <fstream>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_row(0), m_col(0) {
	m_text.push_back("");
	text_it = m_text.begin();
}

StudentTextEditor::~StudentTextEditor()
{
	// TODO
}

bool StudentTextEditor::load(std::string file) {

	ifstream infile(file);
	if (!infile)
		return false;

	reset();
	m_text.clear();

	string s;
	while (getline(infile, s))
	{
		m_text.push_back(s);
	}
	text_it = m_text.begin();

	return true;
}

bool StudentTextEditor::save(std::string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	m_text.clear();
	m_col = 0;
	m_row = 0;
	m_text.push_back("");
	text_it = m_text.begin();
}

void StudentTextEditor::move(Dir dir) {
	switch (dir)
	{
	case (Dir::UP):
		if (m_row != 0)
		{
			text_it--;
			if (m_col >= (*text_it).size())
				m_col = (*text_it).size();
			m_row--;
		}
		break;
	case (Dir::DOWN):
		if (m_row != m_text.size() - 1)
		{
			m_row++;
			text_it++;
			if (m_col >= (*text_it).size())
				m_col = (*text_it).size();
		}
		break;
	case (Dir::LEFT):
		if (m_col != 0)
		{
			m_col--;
		}
		break;
	case (Dir::RIGHT):
		if (m_col != (*text_it).size())
			m_col++;
		break;
	}

}

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	if (m_row == 0 && m_col == 0)
		return;
	if (m_col == 0 || (*text_it).size() == 0)
	{
		string prevLine = *text_it;

		//keep iterator to current line to delete later
		list<string>::iterator temp = text_it;
		text_it--;
		m_col = (*text_it).size();
		(*text_it) += prevLine;
		m_text.erase(temp);
		m_row--;
		return;
	}
	if (atEndOfLine())
	{
		(*text_it) = (*text_it).substr(0, m_col - 1);
		m_col--;
		return;
	}
}

void StudentTextEditor::insert(char ch) {
	if (atEndOfLine())
	{
		(*text_it) += ch;
	}
	else
	{
		*text_it = (*text_it).substr(0, m_col) + ch + (*text_it).substr(m_col, (*text_it).size() - m_col);
	}
	m_col++;
}

void StudentTextEditor::enter() {
	//enter at end of lines
	if (atEndOfLine())
	{
		if (m_row == m_text.size() - 1)	//last line of text
		{
			m_text.push_back("");
			text_it++;
		}
		else
		{
			text_it++;
			m_text.insert(text_it, "");
			text_it--;
		}
	}
	else
	{
		string nextLine = (*text_it).substr(m_col, (*text_it).size() - m_col);
		*text_it = (*text_it).substr(0, m_col);
		text_it++;
		m_text.insert(text_it, nextLine);
		text_it--;
	}

	m_row++;
	m_col = 0;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow > m_text.size())
		return -1;

	lines.clear();

	list<string>::const_iterator it = m_text.begin();
	for (int i = 0; i < startRow; i++)
		it++;
	for (int i = 0; i < numRows && it != m_text.end(); i++)
	{
		string nextLine = (*it);
		lines.push_back(nextLine);
		it++;
	}
	return lines.size();
}

void StudentTextEditor::undo() {
	// TODO
}

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
	m_text.clear();
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
	ofstream outfile(file);

	if (!outfile)
		return false;

	for (list<string>::iterator it = m_text.begin(); it != m_text.end(); it++)
	{
		outfile << *it << endl;
	}

	return true;
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
			m_col--;
		else if (m_row != 0)
		{
			m_row--;
			text_it--;
			m_col = (*text_it).size();
		}
		break;
	case (Dir::RIGHT):
		if (m_col != (*text_it).size())
			m_col++;
		else if (m_row != m_text.size() - 1)
		{
			m_row++;
			text_it++;
			m_col = 0;
		}
		break;
	case (Dir::HOME):
		m_col = 0;
		break;
	case (Dir::END):
		m_col = (*text_it).size();
		break;
	}

}

void StudentTextEditor::del() {
	if (atEndOfLine())
	{
		if (m_text.size() == 1 || m_row == (*text_it).size() - 1)
			return;

		text_it++;
		string nextLine = *text_it;

		//keep iterator to current line to delete later
		list<string>::iterator temp = text_it;
		text_it--;
		(*text_it) += nextLine;
		m_text.erase(temp);

		getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
		return;
	}
	char deletedCh = (*text_it).at(m_col);
	(*text_it) = (*text_it).substr(0, m_col) + (*text_it).substr(m_col + 1, ((*text_it).size() - m_col - 1));
	getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deletedCh);
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

		getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
		
		return;
	}

	char deletedCh = (*text_it).at(m_col - 1);
	(*text_it) = (*text_it).substr(0, m_col - 1) + (*text_it).substr(m_col, ((*text_it).size() - m_col));
	m_col--;
	getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deletedCh);
}

void StudentTextEditor::insert(char ch) {
	if (atEndOfLine())
	{
		if (ch == '\t')
		{
			(*text_it += "    ");
			m_col += 4;
		}
		else
		{
			(*text_it) += ch;
			m_col++;
		}
	}
	else
	{
		if (ch == '\t')
		{
			*text_it = (*text_it).substr(0, m_col) + "    " + (*text_it).substr(m_col, (*text_it).size() - m_col);
			m_col += 4;
		}
		else
		{
			*text_it = (*text_it).substr(0, m_col) + ch + (*text_it).substr(m_col, (*text_it).size() - m_col);
			m_col++;
		}
	}

	if (ch == '\t')
	{
		for (int i = 3; i >= 0; i--)
			getUndo()->submit(Undo::Action::INSERT, m_row, m_col - i, ' ');
	}
	else
		getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() {

	getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
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
	int count;
	string text;
	int prevRow = m_row;

	Undo::Action undoAction = getUndo()->get(m_row, m_col, count, text);

	while (m_row < prevRow)
	{
		text_it--;
		prevRow--;
	}
	while (prevRow < m_row)
	{
		text_it++;
		prevRow++;
	}

	switch (undoAction)
	{
	case Undo::Action::INSERT:
		*text_it = (*text_it).substr(0, m_col) + text + (*text_it).substr(m_col, (*text_it).size() - m_col);
		break;
	case Undo::Action::DELETE:
		*text_it = (*text_it).substr(0, m_col - count) + (*text_it).substr(m_col, (*text_it).size() - m_col);
		m_col -= count;
		break;
	case Undo::Action::SPLIT:
	{
		string nextLine = (*text_it).substr(m_col, (*text_it).size() - m_col);
		*text_it = (*text_it).substr(0, m_col);
		text_it++;
		m_text.insert(text_it, nextLine);
		text_it--;
		text_it--;
		break;
	}
	case Undo::Action::JOIN:
	{
		text_it++;
		string nextLine = *text_it;
		list<string>::iterator temp = text_it;
		text_it--;
		(*text_it) += nextLine;
		m_text.erase(temp);
		break;
	}
	}
}

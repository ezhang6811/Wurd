#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
	m_action.push(action);
	m_row.push(row);
	m_col.push(col);
	m_char.push(ch);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (m_action.top() == INSERT)
		return DELETE;
	if (m_action.top() == DELETE)
	{
		row = m_row.top();
		col = m_col.top();
		return INSERT;
	}
	if (m_action.top() == SPLIT)
		return JOIN;
	if (m_action.top() == JOIN)
		return SPLIT;
}

void StudentUndo::clear() {
	while (!m_action.empty())
		m_action.pop();
}

#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
	m_actions.push(action);
	m_row.push(row);
	m_col.push(col);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (m_actions.top() == INSERT)
		return DELETE;
	if (m_actions.top() == DELETE)
		return INSERT;
	if (m_actions.top() == SPLIT)
		return JOIN;
	if (m_actions.top() == JOIN)
		return SPLIT;
}

void StudentUndo::clear() {
	while (!m_actions.empty())
		m_actions.pop();
}

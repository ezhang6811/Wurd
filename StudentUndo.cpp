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

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
	Action returnAction;

	if (m_action.empty())
		return ERROR;

	count = 1;
	text = "";
	if (m_action.top() == INSERT)
	{
		row = m_row.top();
		col = m_col.top();
		returnAction = DELETE;
	}
	if (m_action.top() == DELETE)
	{
		row = m_row.top();
		col = m_col.top();
		text = m_char.top();
		returnAction = INSERT;
	}
	if (m_action.top() == SPLIT)
		returnAction = JOIN;
	if (m_action.top() == JOIN)
		returnAction = SPLIT;

	m_action.pop();
	m_row.pop();
	m_col.pop();
	m_char.pop();

	return returnAction;
}

void StudentUndo::clear() {
	while (!m_action.empty())
		m_action.pop();
}

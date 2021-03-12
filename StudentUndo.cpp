#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	UndoAction submitAction;
	submitAction.m_action = action;
	submitAction.m_row = row;
	submitAction.m_col = col;
	submitAction.m_char = ch;

	m_actions.push(submitAction);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
	if (m_actions.empty())
		return ERROR;

	count = 1;
	text = "";
	row = m_actions.top().m_row;
	col = m_actions.top().m_col;

	if (m_actions.top().m_action == INSERT)
	{
		//check for batching
		m_actions.pop();

		int prevCol = col;
		while (!m_actions.empty() && m_actions.top().m_action == INSERT && m_actions.top().m_row == row && m_actions.top().m_col == prevCol - 1)
		{
			count++;
			m_actions.pop();
			prevCol--;
		}
		return DELETE;
	}
	if (m_actions.top().m_action == DELETE)
	{
		text = m_actions.top().m_char;
		m_actions.pop();

		int prevCol = col;
		while (!m_actions.empty() && m_actions.top().m_action == DELETE && m_actions.top().m_row == row)
		{
			if (m_actions.top().m_col == prevCol + 1)
			{
				text += m_actions.top().m_char;
				m_actions.pop();
				prevCol++;
			}
			else if (m_actions.top().m_col == prevCol)
			{
				text = m_actions.top().m_char + text;
				m_actions.pop();
			}
			else
				break;
		}
		return INSERT;
	}
	if (m_actions.top().m_action == SPLIT)
	{
		m_actions.pop();
		return JOIN;
	}
	if (m_actions.top().m_action == JOIN)
	{
		m_actions.pop();
		return SPLIT;
	}

	return ERROR;
}

void StudentUndo::clear() {
	while (!m_actions.empty())
		m_actions.pop();
}


//pops top item from each stack
void StudentUndo::popStacks()
{

}

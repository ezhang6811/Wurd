#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:

	std::stack<Action> m_actions;
	std::stack<int> m_row;
	std::stack<int> m_col;
	std::stack<char> m_chars;
};

#endif // STUDENTUNDO_H_

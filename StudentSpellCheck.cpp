#include "StudentSpellCheck.h"
#include <string>
#include <vector>

#include <iostream>
#include <fstream>

using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream dictionaryText(dictionaryFile);

	if (!dictionaryText)
		return false;
	
	trieRoot = new Node;

	string nextWord;
	Node* n;
	while (getline(dictionaryText, nextWord))
	{
		n = trieRoot;

		for (int i = 0; i < nextWord.length(); i++)
		{
			bool nextLetterFound = false;
			for (vector<Node*>::iterator it = n->nextLetter.begin(); it != n->nextLetter.end(); it++)
				if ((*it)->letter == nextWord[i])
				{
					nextLetterFound = true;
					n = *it;
					break;
				}
			if (!nextLetterFound)
			{
				Node* newLetter = new Node;
				newLetter->letter = nextWord[i];
				n->nextLetter.push_back(newLetter);
				n = newLetter;
			}
		}
		Node* endOfWord = new Node;
		endOfWord->letter = '.';
		n->nextLetter.push_back(endOfWord);
	}

	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	//set word to lowercase
	for (int i = 0; i < word.length(); i++)
		word[i] = tolower(word[i]);

	if (inDictionary(word))
		return true;

	//at this point, the word is not in dictionary
	for (int i = 0; i < word.length(); i++)
	{
		string newWord;
		for (char ch = 'a'; ch <= 'z'; ch++)
		{
			newWord = word;
			newWord[i] = ch;
			if (inDictionary(newWord))
			{
				suggestions.push_back(newWord);
			}
		}
		if (suggestions.size() == max_suggestions)
			break;
	}
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	int i = 0;
	while (i < line.length())
	{
		while (i < line.length() && !partOfWord(line[i]))
			i++;
		Position wordPos;
		wordPos.start = i;
		while (i < line.length() && partOfWord(line[i]))
			i++;
		wordPos.end = i - 1;
		if (!inDictionary(line.substr(wordPos.start, wordPos.end - wordPos.start + 1)))
			problems.push_back(wordPos);
	}
}

bool StudentSpellCheck::inDictionary(string word)
{
	Node* n = trieRoot;
	bool inDictionary = true;
	for (int i = 0; i < word.length(); i++)
	{
		bool nextLetterFound = false;
		for (vector<Node*>::iterator it = n->nextLetter.begin(); it != n->nextLetter.end(); it++)
		{
			if (word[i] == (*it)->letter)
			{
				nextLetterFound = true;
				n = *it;
				break;
			}
		}
		if (!nextLetterFound)
		{
			inDictionary = false;
			break;
		}
	}

	if (inDictionary)
	{
		for (vector<Node*>::iterator it = n->nextLetter.begin(); it != n->nextLetter.end(); it++)
		{
			if ((*it)->letter == '.')
				return true;
		}
	}

	return false;
}
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include <locale>         // std::locale, std::tolower

using namespace std;

using std::cin;
using std::cout;
using std::endl;
using std::string;

string strToLower(string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
	return str;
}

vector<string> getWords() {
	// https://g.co/gemini/share/9781bb423d9b
	string filename = "german.txt";
    ifstream file(filename);

	vector<string> words;
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return words;
    }

    string word;

    while (getline(file, word)) {
		vector<string> disallowed = {"ä", "ö", "ü"};
		for (const auto& substring : disallowed) {
			if (word.find(substring) != std::string::npos) {
				continue;
			}
		}
        words.push_back(strToLower(word));
    }

    file.close();
	
	return words;
}

int main() {
  // 'Random seed'
  srand(time(0));
  const vector<string> words = getWords();
  const string word = words[(rand() % words.size() - 1)];
  // cout << word << endl;
  string input;
  string output;
  // https://g.co/gemini/share/3fd54e2828dd
  output.resize(word.length(), '_');

  int guesses = 6;

  for (int guess = 1; guess <= guesses; guess++) {
    cout << "Dies ist der " << guess << ". Versuch: ";
    cin >> input;
	input = strToLower(input);

    if (input.length() != 5) {
      cout << "5 Zeichen, du Depp!" << endl;
      guess--;
      continue;
    }

    if (input == word) {
      cout << "Glückwunsch!" << endl;
      break;
    }
	
	if (std::find(words.begin(), words.end(), input) == words.end()) {
		cout << "Das scheint kein valides Wort zu sein!" << endl;
		guess--;
		continue;
	}

    for(int i = 0; i < 5; i++) {
		if(input[i] == word[i]) {
			cout << "y";
		}
		else if(word.find(input[i]) != std::string::npos) {
			// Im Falle von doppelten Buchstaben im Wort --> O(n^2) geht besser?
			int corrects = 0;
			for(int x = 0; x < 5; x++) {
				if(input[x] == word[x] && input[x] == input[i]) {
					corrects++;
				}
			}
			if(std::count(word.begin(), word.end(), input[i]) > corrects) {
				cout << "!";
			} else {
				cout << "_";
			}
		} else {
			cout << "_";
		}
	}
	cout << endl;
  }
  cout << "Das Wort war: ";
  cout << word << endl;
}

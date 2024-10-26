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

vector<string> getWordsFromFile(string filename) {
	// https://g.co/gemini/share/9781bb423d9b
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
	
	if(words.size() == 0) {
		cout << "Bitte schließe die Datei \"" + filename + "\" und versuche es erneut!";
		throw "Bitte schließe die Datei \"" + filename + "\" und versuche es erneut!";
	}
	
	return words;
}


int main() {
  // 'Random seed'
  srand(time(0));
  
  // Nur normale Wörter in german.txt, 'komischere' Wörter sind in german_valid.txt gespeichert
  const vector<string> words = getWordsFromFile("german.txt");
  const vector<string> validInputs = getWordsFromFile("german_valid.txt");
  
  cout << endl;
  cout << "Ich habe ein zufälliges Wort aus einem Pool von "; cout << words.size(); cout << " Optionen gewählt" << endl;
  cout << "Vom Program anerkannte mögliche Inputs: "; cout <<  validInputs.size() << endl;
  cout << endl;
  
  const string word = words[(rand() % words.size() - 1)];
  string input;

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
	
	// Nicht sehr kulant, benötigt besser gepflegte Wortliste
	if (std::find(validInputs.begin(), validInputs.end(), input) == validInputs.end()) {
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
			// Beispiel
			// 	Wort: katze
			// 	Input: parat
			// 	Output: _y__!
			// 	Output, ohne diesen Check: _y_!!
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

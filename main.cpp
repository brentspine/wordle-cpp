#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include <locale>         // std::locale, std::tolower
#include <stdexcept>

#include <typeinfo>	      // cout << typeid(variable).name() << endl;
#include <unordered_map>

using namespace std;

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::exception;

// https://chatgpt.com/share/671d50a2-a57c-8001-be40-d1c1b5859f46
class WordleException : public std::runtime_error
{
public:
    WordleException(const std::string& err) : std::runtime_error(err) {}       
};

// https://chatgpt.com/share/671d550e-c4c8-8001-9929-257e49f8215c
enum class ConsoleColor {
    Reset = 0,
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    White = 37
};

string getCC(ConsoleColor color) {
    return "\033[" + std::to_string(static_cast<int>(color)) + "m";
}

string getCCReset() {
    return getCC(ConsoleColor::Reset);
}

string strToLower(string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
	return str;
}

vector<string> getWordsFromFile(string filename) {
	// https://g.co/gemini/share/9781bb423d9b
    ifstream file(filename);

	vector<string> words;
    if (!file.is_open()) {
        cerr << "Fehler beim Öffnen der Datei, existiert sie? Angegebene Datei: " << filename << endl;
		throw WordleException("Fehler beim Öffnen der Datei, existiert sie? Angegebene Datei: " + filename);
        return words;
    }

    string word;

    while (getline(file, word)) {
		vector<string> disallowed = {"ä", "ö", "ü", "ß"};
		for (const auto& substring : disallowed) {
			if (word.find(substring) != std::string::npos) {
				continue;
			}
		}
		if(word.length() != 5) continue;
        words.push_back(strToLower(word));
    }

    file.close();
	
	if(words.size() == 0) {
		cerr << "Bitte schließe die Datei \"" << filename << "\"und versuche es erneut!";
		throw WordleException("Bitte schließe die Datei \"" + filename + "\"und versuche es erneut!");
	}
	
	return words;
}


int main() {
  // 'Random seed'
  srand(time(0));
  
  bool checkDictionary = true;
  string wordsFileName;
  cout << "Datei für Wörterauswahl eingeben (Leer für Standard): ";
  // https://stackoverflow.com/questions/4999650/c-how-do-i-check-if-the-cin-buffer-is-empty
  getline(std::cin, wordsFileName);
  if(wordsFileName.length() <= 1) {
	  wordsFileName = "german.txt";
  }
  
  string validInputsFileName;
  cout << "Datei für Dictionary eingeben (Leer für Standard, 'no' = keines): ";
  getline(std::cin, validInputsFileName);
  if(validInputsFileName.length() <= 1) {
	  validInputsFileName = "german_valid.txt";
  }
  if(validInputsFileName == "no") {
	checkDictionary = false;
	cout << "Dictionary wurde deaktiviert!" << endl;
  }
  
  // Nur normale Wörter in german.txt, 'komischere' Wörter sind in german_valid.txt gespeichert
  const vector<string> words = getWordsFromFile(wordsFileName);
  const vector<string> validInputs;
  if(checkDictionary) {
	  vector<string> validInputs = getWordsFromFile(validInputsFileName);
  } else {
	  vector<string> validInputs = {};
  }
  
  cout << endl;
  cout << "Ich habe ein zufälliges Wort aus einem Pool von "; cout << words.size(); cout << " Optionen gewählt" << endl;
  cout << "Vom Program anerkannte mögliche Inputs: "; cout << (validInputs.size() > 0 ? std::to_string(validInputs.size()) : "Alle erlaubt") << endl;
  cout << endl;
  
  const string word = words[(rand() % words.size() - 1)];
  
  const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
  std::unordered_map<char, int> char_map;
  for (char c : alphabet) {
    char_map[c] = 0;
  }
  
  string output_history = "";
  
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
	if (checkDictionary && std::find(validInputs.begin(), validInputs.end(), input) == validInputs.end()) {
		cout << "Das scheint kein valides Wort zu sein!" << endl;
		guess--;
		continue;
	}

	output_history += input;
	string appendAtBack = "    ";
    for(int i = 0; i < 5; i++) {
		if(input[i] == word[i]) {
			// output_history += getCC(ConsoleColor::Green) + input[i];
			appendAtBack += "y";
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
				// output_history +=  getCC(ConsoleColor::Yellow) + input[i];
				appendAtBack += "!";
			} else {
				// output_history += getCC(ConsoleColor::White) + input[i];
				appendAtBack += "_";
			}
		} else {
			// output_history += getCC(ConsoleColor::White) + input[i];
			appendAtBack += "_";
		}
	}
	// output_history += getCCReset() += endl;
	output_history += appendAtBack + "\n";
	cout << output_history;
	cout << endl;
  }
  cout << "Das Wort war: ";
  cout << word << endl;
}

#include "AliveCursor.h"
#include "zypper.h"

#include <ctype.h>
#include <sstream>

using namespace std;

void display_progress (ostream & out, const string& s, int percent) {
  static AliveCursor cursor;

  if (gSettings.machine_readable)
    return;

  if ( percent == 100 )
    out << CLEARLN << cursor.done() << " " << s;
  else
    out << CLEARLN << cursor++ << " " << s;
  // dont display percents if invalid
  if (percent >= 0 && percent <= 100)
    out << " [" << percent << "%]";
  out << flush;
}

// ----------------------------------------------------------------------------

void display_tick (ostream & out, const string& s) {
  static AliveCursor cursor;

  if (gSettings.machine_readable)
    return;

  cursor++;
  out << CLEARLN << cursor << " " << s;
  out << flush;
}

// ----------------------------------------------------------------------------

void display_done (ostream & out, const string& s) {
  static AliveCursor cursor;

  if (gSettings.machine_readable)
    return;

  out << CLEARLN << cursor.done() << " " << s;
  out << flush;
  out << endl;
}

// ----------------------------------------------------------------------------

void display_done (ostream & out) {

  if (gSettings.machine_readable)
    return;

  out << endl;
}

// ----------------------------------------------------------------------------

//template<typename Action>
//Action ...
int read_action_ari (int default_action) {
  // TranslatorExplanation don't translate letters in parentheses!! (yet)
  cout << _("(A)bort, (R)etry, (I)gnore?") << " "; 

  // choose abort if no default has been specified
  if (default_action == -1) {
    default_action = 0;
  }

  // non-interactive mode
  if (gSettings.non_interactive) {
      char c;
      switch (default_action) {
	  case 0: c = 'a'; break;
	  case 1: c = 'r'; break;
	  case 2: c = 'i'; break;
	  default: c = '?';
      }
      // print the answer for conveniecne
      cout << c << endl;
      MIL << "answer: " << c << endl;
      return default_action;
  }

  // interactive mode, ask user
  while (cin.good()) {		// #269263
    char c;
    cin >> c;
    c = tolower (c);
    MIL << "answer: " << c << endl;
    if (c == 'a')
      return 0;
    else if (c == 'r')
      return 1;
    else if (c == 'i')
      return 2;
    cerr << _("Invalid answer. Choose letter a, r, or i.") << endl;
    DBG << "invalid answer" << endl;
  }

  return default_action;
}

// ----------------------------------------------------------------------------

// Read an answer (ynYN)
bool read_bool_answer(const string & question, bool default_answer)
{
  if (!gSettings.machine_readable)
  	cout << CLEARLN << question << " [y/n]: " << flush;

  // non-interactive mode: print the answer for convenience and return default
  if (gSettings.non_interactive)
  {
  	if (!gSettings.machine_readable)
	    cout << (default_answer ? 'y' : 'n') << endl;
    MIL << "answer (default): " << (default_answer ? 'y' : 'n') << endl;
    return default_answer;
  }

  istream & stm = cin;

  string c = "";
  while (stm.good () && c != "y" && c != "Y" && c != "N" && c != "n")
    c = zypp::str::getline (stm, zypp::str::TRIM);

  MIL << "answer: " << c << endl;
  if (c == "y" || c == "Y")
    return true;
  else if (c == "n" || c == "N")
    return false;
  else
  {
    WAR << "could not read answer, returning default: "
        << (default_answer ? 'y' : 'n') << endl;
    return default_answer;
  }
}

// ----------------------------------------------------------------------------

string to_string (zypp::Resolvable::constPtr resolvable) {
  ostringstream ss;
  ss << *resolvable;
  return ss.str ();
}

// Local Variables:
// c-basic-offset: 2
// End:

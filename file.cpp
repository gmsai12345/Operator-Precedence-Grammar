#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

string V[100][2]; // Stores the split grammar
int vi = 0; // Number of lines after splitting
int t = 0;
int s = 0;
int l;
int r;

string FIRSTVT[20][2]; // Stores the FIRSTVT set
string LASTVT[20][2]; // Stores the LASTVT set
string str; // Stores the input grammar
string str_a = "#"; // Stack
string str_b; // Remaining input string
string analysis_table[100][5]; // Grammar analysis process

char table[100][100]; // Operator precedence table

void init_string(string &a) // Removes the first element of a string
{
	for (int i = 1; i <= a.length(); ++i)
	{
		a[i - 1] = a[i];
	}
}

bool is_CHAR(char c) // Checks if the character is an uppercase letter
{
	if (c >= 'A' && c <= 'Z')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_in(int i, string x) // Checks if the last i characters of string x contain non-uppercase letters
{
	bool flag = false;
	for (int j = 0; j < i; j++)
	{
		if (!is_CHAR(x[x.length() - j - 1]))
		{
			flag = true;
		}
	}
	return flag;
}

void split(string a) // Splits the grammar, ensuring no "|"
{
	for (int i = 3; i < a.length(); ++i)
	{
		V[vi][0] = a[0];
		while (a[i] != '|' && i < a.length())
		{
			V[vi][1] += a[i];
			i++;
		}
		vi++;
	}
}

void read_file(string file_path) // Reads the file line by line
{
	fstream f(file_path);
	vector<string> words;
	string line;
	while (getline(f, line))
	{
		words.push_back(line);
	}
	cout << "Input Grammar:" << endl;
	for (int i = 0; i < words.size(); i++)
	{
		cout << words[i] << endl;
		split(words[i]);
	}
}

int find_index(char a) // Finds the index of character a in FIRSTVT or LASTVT
{
	for (int i = 0; i < t; ++i)
	{
		if (FIRSTVT[i][0][0] == a)
		{
			return i;
		}
	}
	return -1;
}

int find_table_index(char a) // Finds the index of character a in the operator precedence table
{
	for (int i = 0; i <= s; ++i)
	{
		if (table[i][0] == a)
		{
			return i;
		}
	}
	return -1;
}

void get_start() // Retrieves non-terminal symbols
{
	for (int i = 0; i < vi; ++i)
	{
		bool flag = true;
		for (int j = 0; j < t; ++j)
		{
			if (FIRSTVT[j][0] == V[i][0])
			{
				flag = false;
			}
		}
		if (flag)
		{
			FIRSTVT[t][0] = V[i][0];
			LASTVT[t][0] = V[i][0];
			t++;
		}
	}
}

void add_firstvt(string b, int a) // Checks if string b is in the FIRSTVT set at index a, adds it if not
{
	for (int s = 0; s < b.length(); ++s)
	{
		bool flag = true;
		char c = b[s];
		if (c <= 'Z' && c >= 'A')
		{
			continue;
		}
		for (int i = 0; i < FIRSTVT[a][1].length(); ++i)
		{
			if (c == FIRSTVT[a][1][i])
			{
				flag = false;
			}
		}
		if (flag)
		{
			FIRSTVT[a][1] += c;
		}
	}
}

void add_firstvt(char c, int a) // Checks if character c is in the FIRSTVT set at index a, adds it if not
{
	bool flag = true;
	for (int i = 0; i < FIRSTVT[a][1].length(); ++i)
	{
		if (c <= 'Z' && c >= 'A')
		{
			continue;
		}
		if (c == FIRSTVT[a][1][i])
		{
			flag = false;
		}
	}
	if (flag)
	{
		FIRSTVT[a][1] += c;
	}
}

void add_lastvt(string b, int a) // Checks if string b is in the LASTVT set at index a, adds it if not
{
	for (int s = 0; s < b.length(); ++s)
	{
		bool flag = true;
		char c = b[s];
		if (c <= 'Z' && c >= 'A')
		{
			continue;
		}
		for (int i = 0; i < LASTVT[a][1].length(); ++i)
		{
			if (c == LASTVT[a][1][i])
			{
				flag = false;
			}
		}
		if (flag)
		{
			LASTVT[a][1] += c;
		}
	}
}

void add_lastvt(char c, int a) // Checks if character c is in the LASTVT set at index a, adds it if not
{
	bool flag = true;
	for (int i = 0; i < LASTVT[a][1].length(); ++i)
	{
		if (c <= 'Z' && c >= 'A')
		{
			continue;
		}
		if (c == LASTVT[a][1][i])
		{
			flag = false;
		}
	}
	if (flag)
	{
		LASTVT[a][1] += c;
	}
}

string get_cur_firstvt(char c, int a) // Retrieves the current character's FIRSTVT and adds it to the FIRSTVT set at index a
{
	string temp;
	for (int i = 0; i < vi; ++i)
	{
		if (c == V[i][0][0])
		{
			if (!(V[i][1][0] <= 'Z' && V[i][1][0] >= 'A'))
			{
				add_firstvt(V[i][1][0], a);
			}
			else
			{
				if (c != V[i][1][0])
				{
					temp = get_cur_firstvt(V[i][1][0], find_index(V[i][1][0]));
					add_firstvt(temp, a);
				}
				if (V[i][1].length() > 2)
				{
					if (!(V[i][1][1] <= 'Z' && V[i][1][1] >= 'A'))
					{
						add_firstvt(V[i][1][1], a);
					}
				}
			}
		}
	}
	return FIRSTVT[a][1];
}

string get_cur_lastvt(char c, int a) // Retrieves the current character's LASTVT and adds it to the LASTVT set at index a
{
	string temp;
	for (int i = 0; i < vi; ++i)
	{
		int s = V[i][1].length();
		if (c == V[i][0][0])
		{
			if (!(V[i][1][s - 1] <= 'Z' && V[i][1][s - 1] >= 'A'))
			{
				add_lastvt(V[i][1][s - 1], a);
			}
			else
			{
				if (c != V[i][1][s - 1])
				{
					temp = get_cur_lastvt(V[i][1][s - 1], find_index(V[i][1][0]));
					add_lastvt(temp, a);
				}
				if (V[i][1].length() > 2)
				{
					if (!(V[i][1][s - 2] <= 'Z' && V[i][1][s - 2] >= 'A'))
					{
						add_lastvt(V[i][1][s - 2], a);
					}
				}
			}
		}
	}
	return LASTVT[a][1];
}


void get_firstvt() // Retrieves the FIRSTVT for all grammars
{
	for (int i = 0; i < t; i++)
	{
		get_cur_firstvt(FIRSTVT[i][0][0], i);
	}
}

void get_lastvt() // Retrieves the LASTVT for all grammars
{
	for (int i = 0; i < t; i++)
	{
		get_cur_lastvt(LASTVT[i][0][0], i);
	}
}

void print_firstvt(string t, string a) // Prints the FIRSTVT of non-terminal t
{
	cout << "FIRSTVT(" << t << ") = {";
	for (int i = 0; i < a.length(); ++i)
	{
		if (i == a.length() - 1)
		{
			cout << a[i];
		}
		else
		{
			cout << a[i] << ",";
		}
	}
	cout << "}" << endl;
}

void print_lastvt(string t, string a) // Prints the LASTVT of non-terminal t
{
	cout << "LASTVT(" << t << ") = {";
	for (int i = 0; i < a.length(); ++i)
	{
		if (i == a.length() - 1)
		{
			cout << a[i];
		}
		else
	{
			cout << a[i] << ",";
		}
	}
	cout << "}" << endl;
}

void init_table() // Adds non-terminal symbols to the operator precedence table
{
	for (int i = 0; i < vi; ++i)
	{
		for (int j = 0; j < V[i][1].length(); ++j)
		{
			bool flag = true;
			for (int k = 0; k < s; ++k)
			{
				if (table[0][k] == V[i][1][j])
				{
					flag = false;
				}
			}
			if (flag && !(V[i][1][j] <= 'Z' && V[i][1][j] >= 'A'))
			{
				table[0][++s] = V[i][1][j];
				table[s][0] = V[i][1][j];
			}
		}
	}
	s++;
}

void add_table(char a, char b, char c) // Adds operator precedence relations
{
	table[find_table_index(a)][find_table_index(b)] = c;
}

void create_table() // Creates the operator precedence table
{
	for (int i = 0; i < vi; ++i)
	{
		for (int j = 0; j < V[i][1].length() - 1; ++j)
		{
			if (!is_CHAR(V[i][1][j]) && !is_CHAR(V[i][1][j + 1]))
			{
				add_table(V[i][1][j], V[i][1][j + 1], '=');
			}
			if (j + 2 < V[i][1].length())
			{
				if (!is_CHAR(V[i][1][j]) && is_CHAR(V[i][1][j + 1]) && !is_CHAR(V[i][1][j + 2]))
				{
					add_table(V[i][1][j], V[i][1][j + 2], '=');
				}
			}
			if (!is_CHAR(V[i][1][j]) && is_CHAR(V[i][1][j + 1]))
			{
				string temp = FIRSTVT[find_index(V[i][1][j + 1])][1];
				for (int k = 0; k < temp.length(); ++k)
				{
					add_table(V[i][1][j], temp[k], '<');
				}
			}
			if (is_CHAR(V[i][1][j]) && !is_CHAR(V[i][1][j + 1]))
			{
				string temp = LASTVT[find_index(V[i][1][j])][1];
				for (int k = 0; k < temp.length(); ++k)
				{
					add_table(temp[k], V[i][1][j + 1], '>');
				}
			}
		}
	}
}

void print_table() // Prints the operator precedence table
{
	cout << "Operator Precedence Table:" << endl;
	for (int i = 0; i < s; ++i)
	{
		for (int j = 0; j < s; ++j)
		{
			cout << table[i][j] << "\t";
		}
		cout << endl;
	}
}

void print(string a) // Prints the string a
{
	for (int i = 0; i < a.length(); ++i)
	{
		cout << a[i];
	}
}

bool check(string str_a) // Checks if str_a contains only one uppercase letter (indicating analysis completion)
{
	int num = 0;
	for (int i = 0; i < str_a.length(); ++i)
	{
		if (!is_CHAR(str_a[i]))
		{
			num++;
		}
	}
	if (num == 2 && str_a.length() == 3)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void analysis() // Operator precedence grammar parsing process
{
	cout << "Enter the string to parse:" << endl;
	cin >> str_b;
	int k = 0;
	while (str_b != "#" || check(str_a))
	{
		str = str_a + str_b;
		bool flag = true;
		int a = 0;
		for (a = 0; a < str_a.length(); ++a)
		{
			if (!is_CHAR(str_a[a]))
			{
				if (flag)
				{
					l = find_table_index(str_a[a]);
					flag = false;
				}
				else
				{
					r = find_table_index(str_a[a]);
				}
			}
		}
		if (table[l][r] == '>')
		{
			int j;
			for (j = str_a.length() - 1; j >= 0; --j)
			{
				if (table[find_table_index(str_a[j - 1])][find_table_index(str_a[j])] == '<')
				{
					break;
				}
			}
			init_string(str_a);
			str_a += "N";
			print(str_a);
			print(str_b);
			cout << "R" << endl;
		}
		else
		{
			init_string(str_b);
			str_a += str_b[0];
			print(str_a);
			print(str_b);
			cout << "I" << endl;
		}
	}
	print(str_a);
	print(str_b);
	cout << "Analysis Completed" << endl;
}

int main()
{
	read_file("in.txt"); // Reads the grammar file
	get_start(); // Retrieves non-terminal symbols
	get_firstvt(); // Retrieves FIRSTVT set
	get_lastvt(); // Retrieves LASTVT set
	for (int i = 0; i < t; ++i)
	{
		print_firstvt(FIRSTVT[i][0], FIRSTVT[i][1]); // Prints the FIRSTVT set
	}
	for (int i = 0; i < t; ++i)
	{
		print_lastvt(LASTVT[i][0], LASTVT[i][1]); // Prints the LASTVT set
	}
	init_table(); // Adds non-terminal symbols to the operator precedence table
	create_table(); // Creates the operator precedence table
	print_table(); // Prints the operator precedence table
	analysis(); // Operator precedence grammar parsing process
	return 0;
}

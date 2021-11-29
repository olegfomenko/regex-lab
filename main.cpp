#include <fstream>
#include <iostream>
#include <regex>
#include <map>

using namespace std;

const pair<string, string> parts[] = {
        {"([1-4])",                        "Year"},
        {"([a-zA-Z]|[0-9]|\\-){1,6}",      "Group"},
        {"(1[0-5]|[1-9])",                 "Week"},
        {"([a-zA-Z]|[0-9]|['\\- ]){1,28}", "Subject"},
        {"([A-Za-z'\\-]){1,27}",           "First name"},
        {"([A-Za-z'\\-]){1,20}",           "Last name"},
        {"(Le|P|S|Lab)",                   "Subject type"},
        {"([1-5])",                        "Day"},
        {"([1-4])",                        "Lesson number"},
        {"([1-9][0-9]*)",                  "Room"},
};

const string separator = "([;\\:\\?])";
const string whitespace = "(\\s*)";

const int REMOVE_PART = 6;
const string TMP_FILE = "temp.txt";

inline regex make_regex(int start = 0);

bool is_file_matched(const regex &match_regex, const string &path);

void
remove_suffix_with_save(const regex &replace_regex, const regex &save_regex, const string &input_path,
                        const string &output_path);

void
replace(const regex &replace_regex, const string &replace_str, const string &input_path, const string &output_path);

int main() {
    regex match_regex = make_regex();

    cout << "Enter input file path: " << '\n';
    string input_file;
    cin >> input_file;

    cout << "Enter output file path: " << '\n';
    string output_file;
    cin >> output_file;

    if (is_file_matched(match_regex, input_file)) {
        cout << "File matches format!" << '\n';
    } else {
        cout << "File does not match format!" << '\n';
        return 0;
    }

    regex remove_regex = make_regex(REMOVE_PART);
    regex save_regex = make_regex(REMOVE_PART + 1);

    remove_suffix_with_save(remove_regex, save_regex, input_file, TMP_FILE);

    regex replace_separator(separator);
    replace(regex(separator), ";", TMP_FILE, output_file);

    return 0;
}

inline regex make_regex(int start) {
    string regex_str = whitespace;

    for (int i = start; i < size(parts); ++i) {
        auto&[reg, name] =  parts[i];
        regex_str += reg;
        regex_str += whitespace;

        if (i != size(parts) - 1) {
            regex_str += separator;
            regex_str += whitespace;
        }
    };

    regex_str += "$";
    return regex(regex_str);
}

bool is_file_matched(const regex &match_regex, const string &path) {
    ifstream input_file(path);
    string line;
    bool match_flag = true;

    do {
        getline(input_file, line);

        if (!regex_match(line, match_regex)) match_flag = false;

    } while (!input_file.eof() && match_flag);

    input_file.close();
    return match_flag;
}

void
remove_suffix_with_save(const regex &replace_regex, const regex &save_regex, const string &input_path,
                        const string &output_path) {
    ifstream input_file(input_path);
    ofstream output_file(output_path);

    string input_line;
    string output_line;
    do {
        getline(input_file, input_line);

        smatch match;
        regex_search(input_line, match, save_regex);
        string to_save = match[0];

        output_line = regex_replace(input_line, replace_regex, "");
        output_line += to_save;

        if (!output_line.empty()) output_file << output_line << '\n';
    } while (!input_file.eof());

    input_file.close();
    output_file.close();
}

void
replace(const regex &replace_regex, const string &replace_str, const string &input_path, const string &output_path) {

    ifstream input_file(input_path);
    ofstream output_file(output_path);

    string input_line;
    string output_line;
    do {
        getline(input_file, input_line);
        output_line = regex_replace(input_line, replace_regex, replace_str);
        if (!output_line.empty()) output_file << output_line << '\n';
    } while (!input_file.eof());

    input_file.close();
    output_file.close();
}

// /Users/olegfomenko/Documents/Projects/CLion/auto_3_4/input.txt
// /Users/olegfomenko/Documents/Projects/CLion/auto_3_4/output.txt
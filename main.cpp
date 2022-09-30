#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "btree.h"
#include <regex>

using namespace std;
int main() {
    cout << "Reading game data and constructing tree" << endl;

    BTree<8, int, string, string, float, int> tree;

    ifstream infile(R"(C:\Users\marhl\dsv\tree_hugging\data\algorithms_part_dataset.csv)");
    std::string header;
    getline( infile, header);
    int line_count = 0;
    for( std::string line; getline( infile, line); ) {

        // Remove start and end quote & ";" characters
        // Some lines are not terminated with ";"
        if (line.back() == ';') {
            line.pop_back();
        }
        line.pop_back();
        line.erase(line.begin());

        // Hardcoded parsing logic :(
        // ID,GAME_NAME,BEHAVIOUR,PLAY_PURCHASE,NONE
        string column_item;
        istringstream line_stream(line);

        std::regex rgx("\"\",?");
        std::sregex_token_iterator iter(line.begin(),
                                        line.end(),
                                        rgx,
                                        -1);
        std::sregex_token_iterator end;

        try
        {
            getline(line_stream, column_item, ',');
            auto id = stoi(column_item);
            iter++;
            auto game = *iter;
            iter++;
            //cout << "remaining text: " << *iter << endl;
            line_stream = istringstream(*iter);
            getline(line_stream, column_item, ',');
            auto behaviour = column_item;
            getline(line_stream, column_item, ',');
            //cout << column_item << endl;
            auto play_purchase= stof(column_item);
            getline(line_stream, column_item, ',');
            //cout << "\"" << column_item << "\"" << endl;
            auto none = stoi(column_item);

            auto row = tuple<int, string, string, float, int>(id, game, behaviour, play_purchase, none);
            node_data<int, string, string, float, int> data(row);
            tree.insert(data);

        } catch (std::invalid_argument&) {
            cout << "failing line: " << endl << line << endl;
            break;
        }

        if (line_count % 1000 == 0)
        {
            cout << "parsed: " << line_count << " lines" << endl;
        }

        line_count++;
    }

    /*auto traveller = [] (const node_data<int, string, string, float, int>&){
    };
    */

    cout << std::flush;

    return 0;
}

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <cstdio>
#include <memory>
#include <map>
#include <string>
#include <stdexcept>
#include <Windows.h>
using namespace nlohmann;
using namespace std;

inline string getAnswer(json data, int length) {
    srand(time(NULL));
    return data[rand() % length];
}

inline void color(int status) {
    if (status == 2)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 39);
    else if (status == 1)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 103);
    else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

inline vector<string> split(string str, string sub) {
    vector<string> result;
    string::size_type begin, end;

    end = str.find(sub);
    begin = 0;

    while (end != string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end - begin));
        }
        begin = end + sub.size();
        end = str.find(sub, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;
}

inline void print(vector<string> statuses, array<array<int, 5>, 6> status, int guess_times) {
    cout << statuses[0] << "\n" << statuses[1] << "\n";
    for (int times = 0; times < guess_times; times++) {
        for (int i = 0; i < 5; i++) {
            switch (status.at(times).at(i))
            {
            case 0:
                color(0);
                cout << statuses[times + 2].at(i);
                break;
            case 1:
                color(1);
                cout << statuses[times + 2].at(i);
                break;
            case 2:
                color(2);
                cout << statuses[times + 2].at(i);
                break;
            default:
                break;
            }
        }
        color(0);
        cout << "\n";
    }
}

array<int, 5> check(string rep, string ans) {
    map<char, int> rep_map;
    for (auto c : rep) {
        if (rep_map.count(c) != 0) rep_map[c]++;
        else rep_map[c] = 1;
    }

    array<int, 5> result{ 0 };
    for (int i = 0; i < 5; i++) {
        int pos = ans.find(rep.at(i));
        if (pos != -1) {
            if (rep_map[rep.at(i)] == 1) {
                if (pos == i) result[i] = 2;
                else result[i] = 1;

                rep_map[rep.at(i)] = 0;
            }
            else {
                if (rep_map[rep.at(i)] != 0) {
                    while ((pos = ans.find(rep.at(i), pos)) != -1 && rep_map[rep.at(i)] != 0) {
                        if (pos == i) result[i] = 2;
                        else result[i] = 1;

                        rep_map[rep.at(i)]--;
                    }
                }
            }
        }
    }
    return result;
}

int main()
{
    fstream fs;
    string json_str;
    string rep, ans;
    array<array<int, 5>, 6> status{ 0 };

    fs.open("./Wordle.json", ios::in);
    fs >> json_str;

    auto data = json::parse(json_str);
    sort(data.begin(), data.end());

    string status_str = "Wordle!!! (CPP VERSION)\nStarting Guess:\n";
    int length = data.end() - data.begin();
    int guess_times = 0;

    ans = getAnswer(data, length).c_str();

    cout << status_str << "input: ";
    do {
        cin >> rep;
        if (rep.length() > 5 || rep.length() < 5 || (find(data.begin(), data.end(), rep) == data.end())) {
            system("cls");
            print(split(status_str, "\n"), status, guess_times);
            cout << "Please input again: ";
            continue;
        }
           
        status[guess_times] = check(rep, ans);
        status_str += rep + "\n";

        guess_times++;
        if (rep == ans || guess_times == 6) break;

        system("cls");
        print(split(status_str, "\n"), status, guess_times);
        cout << "input: ";
    } while (true);

    system("cls");
    print(split(status_str, "\n"), status, guess_times - 1);
    cout << "\nans: " << ans
         << "\nYUKINEKO WORDLE " << guess_times << "/6\n";

    system("pause");
}
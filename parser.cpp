#include <bits/stdc++.h>

#define endl "\n"

using namespace std;


void calc_first(map<string, set<string>> &first, vector<pair<string, string>> &prod){
    //initially insert all the terminal and epsilon to the first of a non-terminal
    for(auto &x : prod){
        if(!isupper(x.second[0])){
            string str;
            str += x.second[0];
            first[x.first].insert(str);
        }

        if(x.second=="#"){
            first[x.first].insert(x.second);
        }
    }

    //now repeat the process of computing first recursively till there is no change in first of any of the non-terminals
    bool flag = 1;
    while(flag){
        flag = 0;
        //for each production rule
        for(auto &x : prod){
            int prevSize = first[x.first].size();
            //char ch = typeStr(x.second, first);
            bool flag2 = 1;
            //for each tokens in RHS of production rule
            for(auto &y : x.second){
                string st3(1,y);
                if(isupper(y)){
                    //if there is non-terminal A in first of a non-terminal X then add first(A) to first(X) and repeat it until you hit a terminal or a non-terminal wihch don't have epsilon in its first 
                    if(first[st3].find("#")!=first[st3].end()){
                        set<string> firstTemp(first[st3]);
                        firstTemp.erase("#");
                        first[x.first].insert(firstTemp.begin(), firstTemp.end());
                    }
                    else{
                        first[x.first].insert(first[st3].begin(), first[st3].end());
                        flag2 = 0;
                        break;
                    }
                }
                else{
                    first[x.first].insert(st3);
                    flag2 = 0;
                    break;
                }
            }
            if(flag2){
                first[x.first].insert("#");
            }
            int curSize = first[x.first].size();
            if(prevSize!=curSize){
                flag = 1;
            }
        }

    }

}

void calc_follow(map<string, set<string>> &follow, map<string, set<string>> &first, vector<pair<string, string>> &prod, string strt){
    //add $ to the follow of start symbol
    follow[strt].insert("$");

    //to store all the non-terminals
    set<string> non_terminals;

    //initially add t to the follow of A for all the production of type S->..At..
    for(auto &x : prod){
        //to store all the non-terminals
        non_terminals.insert(x.first);
        for(unsigned int i=0; i<x.second.size(); ++i){
            if(i<x.second.size()-1 && isupper(x.second[i]) && !(isupper(x.second[i+1]))) {
                string sTemp(1,x.second[i+1]);
                string sTemp2(1,x.second[i]);
                follow[sTemp2].insert(sTemp);
            }
        }
    }

    //add follow of LHS to all the non-terminals at the last of RHS in a rule
    for(auto &x : prod){
        if(isupper(x.second[x.second.size()-1])){
            string sTemp(1,x.second[x.second.size()-1]);
            follow[sTemp].insert(follow[x.first].begin(), follow[x.first].end());
        }
    }


    //compute follow recursively untill there is no change in follow of any non-terminal
    bool flag = 1;
    while(flag){
        flag = 0;
        //for each non-terminal
        for(auto &x : non_terminals){
            int prevSize = follow[x].size();

            //for all the productions
            for(auto &y : prod){
                string str = y.second;
                string stl = y.first;

                //if a non-terminal X is at the last in RHS then add follow(LHS) to follow(X)
                string sTemp(1,str[str.size()-1]);
                if(x==sTemp){


                    follow[x].insert(follow[stl].begin(), follow[stl].end());
                }

                //compute follow(X) till the end of the production in RHS considering all the cases
                bool seenX = 0;
                bool flag2 = 0;
                for(unsigned int k=0; k<str.size(); ++k){
                    if(seenX){

                        //debug
                        //if(x[0]=='S') cout << "\nits wrong\n";

                        string stmp(1,str[k]);
                        //if again X is found after X then reset everything and continue to next iteration
                        if(stmp==x){
                            flag2 = 1;
                            continue;
                        }
                        //if follow of X is a terminal then add that and set seenX=0 and if it's non-terminal with no epsilon then add first(non-terminal) to follow(X) and 
                        if(!(isupper(str[k])) || first[stmp].find("#")==first[stmp].end()){
                            flag2 = 0;
                            if(isupper(str[k])){
                                //set<string> setemp(first[stmp]);
                                follow[x].insert(first[stmp].begin(), first[stmp].end());
                            }
                            else{
                                follow[x].insert(stmp);
                            }
                            seenX = 0;
                            //break;
                        }
                        //else add first(non-terminal) excluding epsilon to follow(X) and repeat until above condition or end of the string is reached
                        else{
                            set<string> setmp(first[stmp]);
                            setmp.erase("#");
                            follow[x].insert(setmp.begin(), setmp.end());
                        }
                    }
                    else if(str[k]==x[0]){
                        seenX = 1;
                        flag2 = 1;
                    }
                }
                // if all the non-terminals after X contain epsilon then add follow of LHS to follow(X)
                if(flag2){
                    follow[x].insert(follow[stl].begin(), follow[stl].end());

                }

            }

            int curSize = follow[x].size();
            if(prevSize!=curSize){
                flag = 1;
            }

        }

    }


}

void displayFirst(map<string, set<string>> &first){
    for(auto &x : first){
        cout << "\nfirst(" << x.first << ") = { ";
        for(auto &y : x.second){
            cout << y << ", ";
        }
        cout << "}\n";
    }

}

void displayFollow(map<string, set<string>> &follow){
    for(auto &x : follow){
        cout << "\nfollow(" << x.first << ") = { ";
        for(auto &y : x.second){
            cout << y << ", ";
        }
        cout << "}\n";
    }

}

void calc_table(map<string, set<string>> &follow, map<string, set<string>> &first, vector<pair<string, string>> &prod, map<pair<string, string>, string> &table){
    for(auto &x : prod){
        string str = x.second;
        string stl = x.first;

        for(unsigned int i=0; i<str.size(); ++i){
            string stemp(1,str[i]);
            if(!isupper(str[i]) && str[i]!='#'){
                table[{stl,stemp}] = str;
                break;
            }
            else if(isupper(str[i])){
                if(first[stemp].find("#")==first[stemp].end()){
                    for(auto &y : first[stemp]){
                        table[{stl,y}] = str;
                    }
                    break;
                }
                else{
                    for(auto &y : first[stemp]){
                        if(y!="#"){
                            table[{stl,y}] = str;
                        }
                    }
                }
            }
        }

        if(first[stl].find("#")!=first[stl].end()){
            for(auto &y : follow[stl]){
                table[{stl,y}] = str;
            }
        }


    }

}

void displayTable(map<pair<string, string>, string> &table){
    for(auto &x : table){
        cout << x.first.first << " " << x.first.second << " = " << x.second << "\n";
    }
    cout << "\n";
}

void checkValid(map<pair<string, string>, string> &table){
    string strt;
    cout << "enter the start symbol\n";
    cin >> strt;
    stack<string> st;
    stack<string> sti;
    st.push("$");
    st.push(strt);
    string expr;
    cout << "enter the expression to be matched\n";
    cin >> expr;
    reverse(expr.begin(), expr.end());
    sti.push("$");
    for(auto &x : expr){
        string temp;
        temp += x;
        sti.push(temp);
    }
    bool flag = 1;
    while(st.top()!="$"){
        string a=st.top(), b=sti.top();
        if(a==b){
            st.pop();
            sti.pop();
        }
        else if(table.find(make_pair(a, b))!=table.end()){
            string rhs = table[make_pair(a,b)];
            reverse(rhs.begin(), rhs.end());
            st.pop();
            if(rhs!="#"){
                for(auto &x : rhs){
                    string temp;
                    temp += x;
                    st.push(temp);
                }
            }
        }
        else{
            flag = 0;
            break;
        }

    }
    if(flag){
        cout << "Valid String!!\n";
    }
    else cout << "Invalid String!!\n";

}

int main()
{
    vector<pair<string, string>> prod;
    int n; 
    cout << "enter number of production rules\n";
    cin >> n;
    cout << "enter production rules (one rule on each line) separated by space e.g for rule (S->AcB) type \"S AcB\" without quotes followed by enter \n";
    string st1, st2;
    string strt;
    // input for the start production 
    cin >> st1 >> st2;
    prod.push_back(make_pair(st1, st2));
    strt = st1;
    n--;
    //input for the rest of the production rules
    while(n--){
        cin >> st1 >> st2;
        prod.push_back(make_pair(st1, st2));
    }

    // memory to store first and follow set
    map<string, set<string>> first;
    map<string, set<string>> follow;

    // function to calculate first 
    calc_first(first, prod);
    cout << "************************FIRST**********************\n\n";
    displayFirst(first);
    cout << "\n\n";

    // function to calculate follow 
    calc_follow(follow, first, prod, strt);
    cout << "************************FOLLOW**********************\n\n";
    displayFollow(follow);
    cout << "\n\n";

    map<pair<string, string>, string> table;
    calc_table(follow, first, prod, table);
    displayTable(table);
    checkValid(table);
    //solve(table);

    return 0;
}



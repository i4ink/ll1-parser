#include<unistd.h>
#include <regex>
#include<bits/stdc++.h>
using namespace std;
int main()
{
    string s,s1,s2;
    //char b;
    int num,flag=0;
    //cout<<"Enter the number of string/LHS in grammer:";
    cin>>num;
    map<string,list<string>>m,m1;
    vector<char>ch(26);
    for(int z=0;z<num;z++)
    {   s1="",s2="";

        list<string>l,l1,l2;                  ///l1 for E'  , l2 for E
        cin>>s;
        //cout<<s<<"\n"; 
        string inn="";
        //cout<<"string:"<<s<<"\n";
        for(auto x:s)                                      ///tokenising the symbol like E+T , E-T
        {   
            if(x==':' || x=='|')
            {
                l.push_back(inn);
                inn="";
            }
            else
                inn+=x;
        }
        l.push_back(inn);
    //   list<string>::iterator li;
    //    for(auto x:m)
    //     {
    //         cout<<x.first<<" : ";
    //         for(auto &i:x.second)
    //         {
    //             cout<<i<<" ";
    //         }
    //         cout<<"\n";
    //     }     
        for(auto x:m)
        {    //cout<<x.first<<"\n";                                      /////////////For the given input check wheather the expression has some reapeatetive symbol from above string 
                for(auto li=l.begin();li!=l.end();++li) 
                {
                    size_t found = (*li).find(x.first);                                                
                    s1=*li;
                    if ((found != string::npos) && (isalpha(s1[found+1]))) 
                    { 
                        for(auto el:x.second)
                        {
                            s1=*li;
                            s1.erase(found,1);
                            s1.insert(found,el);
                            // cout<<"orig:"<<(*li)[found]<<" edit: "<<s1<<"\n";
                            //sleep(10);
                            l.push_back(s1);
                        }
                        li=l.erase(li);
                        --li;
                    }
                    
                }
        }
        // for(auto x:l)
        // cout<<x<<" ";
        // cout<<"\n";
        inn=l.front();
        l.pop_front();
        m[inn]=l;
    
        // cout<<inn<<" "<<inn.erase(0,1);
        list<string>x=m[inn];
        flag=0;
       // cout<<inn<<" ";
        for (auto& i:x)
        {   s1=i[0];
            
            if(s1==(inn))
            {
                i.erase(0,1);
                i+=s1+"'";
                l1.push_back(i);
                flag=1;
            }
            else if((i.find(inn)!=string::npos) && flag==1){
               
                l2.push_back(i);
            }
            else if(flag){
                i+=inn+"'";
                //cout<<i<<"\n";
                
                l2.push_back(i);
            }
            else{
                l2.push_back(i);
            }
            //   cout<<i<<" "<<"\n";
        }
        if(l2.size())
        { 
            m1[inn]=l2;
            m[inn]=l2;
            l2.clear();
            //s2.pop_back();
        }
        if(l1.size())
        { l1.push_back("epsilon");
            m1[inn+"'"]=l1;
        l1.clear();}
        
    } 
    string strt; cin >> strt;
    string expr; cin >> expr;

    //int count=0;
    vector<pair<string, string>> vp;
    for(auto &x : m1){
        for(auto &y : x.second){
            if(y=="epsilon"){
                vp.push_back(make_pair(x.first, "#"));
            }
            else
            vp.push_back(make_pair(x.first, y));
        }
    }

    set<string> ss;
    int a[26] = {0};
    for(auto &x : vp){
        a[x.first[0]-'A'] = 1;
        if(x.first[1]=='\''){
            ss.insert(x.first);
        }
    }

    for(auto &x : ss){
        string str;
        for(int i=0; i<26; ++i){
            if(a[i]==0){
                str += 'A'+i;
                a[i] = 1;
                break;
            }
        }
        for(auto &y : vp){
            if(y.first == x){
                y.first = str;
            }
            y.second = regex_replace(y.second, regex(x), str);
        }
    }

    vector<pair<string, string>> start;
    for(auto &x : vp){
        if(x.first == strt){
            start.push_back(x);
        }
    }

    cout << vp.size() <<endl;
    for(auto &x : start){
        cout << x.first << " " << x.second << endl;
    }
    for(auto &x : vp){
        if(x.first!=strt)
        cout << x.first << " " << x.second << endl;
    }
    //cout << endl;
    cout << strt << endl;
    cout << expr << endl;

    /*
    for(auto x:m1)
        {   
             cout<<x.first<<" : ";
            for(auto &i:x.second)
            {  
               cout<<i<<"\n";
               
            }
            cout<<"\n";
        }
    */
}

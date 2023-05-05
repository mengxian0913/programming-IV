/*********************************************************
# File Name: a.cpp
# Author: Vincent Yang
# Mail: mengxian0913@gmail.com
# Created Time: 一  5/ 4 15:11:39 2023
*********************************************************/
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ff first
#define ss second
#define fastIO cin.tie(nullptr)->sync_with_stdio(false);
#define INF 0x7FFFFFFF
#define pb push_back
#define all(aa) aa.begin(),aa.end()
#define MOD 1e9+7

int val[9] = {8, 7, 6, 5, 4, 3, 2, 1, 0};
bool poker[5][14]; // 黑桃、愛心、菱形、梅花 from 1 to 4 of index 1
int val_poker[5][14];
map<string, int> digits, colors;

void make_val_of_poker(){
    int now = 1;
    int Ace = 1000;
    for(int i=1; i<=13;i++){
        for(int k=4;k>=1;k--){
            if(i == 1) val_poker[k][i] = Ace++;
            else val_poker[k][i] = now++;
        }
    }
    return;
}

void prev_translate(){

    digits["Ace"] = 1;
    digits["Two"] = 2;
    digits["Three"] = 3;
    digits["Four"] = 4;
    digits["Five"] = 5;
    digits["Six"] = 6;
    digits["Seven"] = 7;
    digits["Eight"] = 8;
    digits["Nine"] = 9;
    digits["Ten"] = 10;
    digits["Jack"] = 11;
    digits["Queen"] = 12;
    digits["King"] = 13;
    
    
    colors["Spades"] = 1;
    colors["Hearts"] = 2;
    colors["Diamonds"] = 3;
    colors["Clubs"]  = 4;
    
    return;
}


int translate(string s1){

    stringstream cards(s1);
    string card;
    int digit = 0, color = 0;
    int max_val = -INF;
    while(cards >> card){
        if(card == "of"){
            continue;
        }

        if(digits.count(card)){
            digit = digits[card];        
        }

        else{
            color = colors[card];
            poker[color][digit] = true;
            max_val = max(val_poker[color][digit], max_val);
        }
    }
    return max_val;
}


bool Straight_flush(){
    bool ch = false;
    for(int i=1;i<=4;i++){
        for(int j=1;j<=11;j++){
            int cnt = 0;
            for(int k = j; k <= j+4; k++){
                if( k > 13 ){
                    if(poker[i][k%13] == false) break;
                }   

                else{
                    if(poker[i][k] == false) break;
                }

                cnt++;
            }

            if(cnt == 5){
                return 1;
            }
        }
    }

    return 0;
}


bool Four_of_kind(){

    for(int i=1;i<=13;i++){
        int cnt = 0;
        for(int j=1;j<=4;j++){
            if(poker[j][i] == false) break;
            cnt++;
        }
        if(cnt == 4) return 1;
    }

    return 0;
}


bool Full_house(){

    bool d1 = 0, d2 = 0;
    for(int i=1;i<=13;i++){
        int cnt = 0;
        for(int j=1;j<=4;j++){
            if(poker[j][i]) cnt++;
        }

        if(cnt == 3) d1 = 1;
        else if(cnt == 2) d2 = 1;
    }

    if(d1 && d2) return 1;
    return 0;
}


bool flush(){
    for(int i=1;i<=4;i++){
        int cnt = 0;
        for(int j=1;j<=13;j++){
            if(poker[i][j]) cnt++;
        }
        if(cnt == 5) return 1;
    }
    return 0;
}


bool Straight(){

    int cnt = 0;

    for(int i=1;i<=11;i++){
        cnt = 0;
        for(int j=i;j<=i+4;j++){
            for(int k=1;k<=4;k++){
                if(j > 13){
                    if(poker[k][j%13]){
                        cnt++;
                        break;
                    }
                }

                else{
                    if(poker[k][j]){
                        cnt++;
                        break;
                    } 
                }
            }
        }
        if(cnt == 5) return 1;
    }
    return 0;
}


bool Three_of_kind(){

    for(int i=1;i<=13;i++){
        int cnt = 0;
        for(int k=1;i<=4;k++){
            cnt += poker[k][i];
        }
        if(cnt == 3) return 1; 
    }
    return 0;
}


bool Two_pair(){

    int cnt1 = 0, cnt2 = 0;
    for(int i=1;i<=13;i++){
        cnt1 = 0;
        for(int k=1;k<=4;k++){
            cnt1 += poker[k][i];
        }
        if(cnt1 == 2) cnt2++;
    }

    if(cnt2 == 2) return 1;
    return 0;
}

bool One_pair(){

    int cnt = 0;
    for(int i=1;i<=13;i++){
        cnt = 0;
        for(int k=1;k<=4;k++){
            cnt += poker[k][i];
        }
        if(cnt == 2) return 1;
    }
    return 0;
}

bool (*command[])() = {Straight_flush, Four_of_kind, Full_house, flush, Straight, Three_of_kind, Two_pair, One_pair};


int find_mode(){
    
    for(int i=0;i<8; i++){
        bool ans = command[i]();
        if(ans){
            return val[i];
        }
    }

    return val[8];
}

void solve(string s1, string s2){
    pair<int, int> player1 = {-1 ,-1}, player2 = {-1, -1};
    int win = 1; 
    

    player1.ff = translate(s1);
    player1.ss = find_mode();
    memset(poker, false, sizeof(poker));

    player2.ff = translate(s2);
    player2.ss = find_mode();

    if(player1.ss < player2.ss){
        win = 2;
    }

    else if(player1.ss == player2.ss){
        if(player1.ff < player2.ff){
            win = 2;
        }
    }

    cout << "Player " << win << " Win the Game.\n\n";

    return;
}

signed main(){
    fastIO
    prev_translate();
    make_val_of_poker();

    string s1, s2;
    int t;
    cin >> t;
    cin.ignore();

    //while (getc(stdin) != '\n');

    while(t--){
        memset(poker, false, sizeof(poker));
        getchar();
        getline(cin, s1);
        getchar();
        getline(cin, s2);
        solve(s1, s2);
    }
    return 0;
}


#include<iostream>
#include<vector>//vector容器的需要
#include <cstdlib> // rand(), srand()
#include <ctime> //time()
#include <limits> // 用于 numeric_limits
#include<string>
#include<fstream>
#include<sstream>
#include <algorithm>
using namespace std;

struct Player{
public:
   string name;
   int guessCount;
};


int generateRandomNumber(int min, int max) {//让电脑生成随机数的函数
     return rand() % (max - min + 1)+min;//原理：a%b得到的结果只能小于b
}

int getPlayerGuess() { //保证用户输入的是有效正确的数字
    int guess;
    cout << "请输入你的猜测: ";
    while (!(cin >> guess)) {
        cin.clear(); //不能成功输入的时候，需要这个成员函数进行重置错误标志位
        cin.ignore(numeric_limits<streamsize>::max(), '\n');//忽略用户的所有输入，直到遇到回车
        cout << "输入无效，请输入一个整数: ";
    }
    return guess;
}

bool compareGuess(int guess, int target, int &guessCount) {//guessCount用于记录猜的次数，猜数字的核心函数
    guessCount++;
    if (guess > target) {
        cout << "太大" << endl;
        return false;
    } else if (guess < target) {
        cout << "太小" <<endl;
        return false;
    } else {
        cout << "恭喜你，猜对了！" <<endl;
        return true;
    }
}

void loadLeaderboard(vector<Player>& leaderboard) {  //函数的作用是读取文件数据，加载到leaderboard的容器里
    ifstream file("leaderboard.txt");    //创建文件输入流对象file，尝试打开 "leaderboard.txt" 文件
    if (!file.is_open()) {    //is_open函数检查文件是否打开
        cout << "首次运行，暂无历史记录。"<<endl;
        return;
    }
    string line;
    while (getline(file, line)) { //把文件流中的数据给到line
        istringstream iss(line);//把line的数据给到iss
        string name;
        int count;
        if (iss >> name >> count) { //名字会被给到name，数字给到count
            leaderboard.push_back({name, count});  // 创建了一个对象，存在了leaderboard这个容器里
        }
    }
    file.close();//关闭文件
}

void updateLeaderboard(vector<Player>& leaderboard, const string& playerName, int guessCount) {
    leaderboard.push_back({playerName, guessCount});  // 直接初始化 Player 对象
    ofstream file("leaderboard.txt");//打开这个leaderboard文件
    if (file.is_open()) {
        for (const auto& player : leaderboard) { //范围for循环，历遍容器里的所有player对象
            file << player.name << " " << player.guessCount << endl; //把数据放入磁盘文件
        }
        file.close();
    } else {
        cerr << "警告：无法保存排行榜数据！"<<endl;
    }
}

void displayLeaderboard(const vector<Player>& leaderboard) {  
    cout << "=== 排行榜 ==="<<endl;
    vector<Player> sortedLeaderboard = leaderboard;
    sort(sortedLeaderboard.begin(), sortedLeaderboard.end(),   //sort(起始迭代器, 结束迭代器, 比较函数);guesscount小的排前面
        [](const Player& a, const Player& b) {
            return a.guessCount < b.guessCount;
        });
    for (size_t i = 0; i < sortedLeaderboard.size(); i++) {
        cout << i+1 << ". " << sortedLeaderboard[i].name 
                  << " - " << sortedLeaderboard[i].guessCount << "次"<<endl;
    }
}

int main(){
    cout<<"欢迎来到猜数字游戏"<<endl;
    srand(time(nullptr));//设置种子，保证随机
    vector<Player> leaderboard;//使用动态数组容器，是一个存储Player这个类的数组，名字叫leaderboard
    loadLeaderboard(leaderboard);
    string playerName;
    cout << "请输入你的名字: ";
    cin >> playerName;
    int targetNumber=generateRandomNumber(1,100);//设置随机数的范围，此处得到的范围是1-100
    int guessCount = 0;//设置最开始的猜测次数是0
    while(1){
    int playerGuess=getPlayerGuess();
        if (compareGuess(playerGuess,targetNumber,guessCount)){
            updateLeaderboard(leaderboard, playerName, guessCount);
            displayLeaderboard(leaderboard);
           break;
        } 
    }
    return 0;
    }
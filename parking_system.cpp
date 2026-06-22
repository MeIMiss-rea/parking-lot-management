#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

// 车辆信息结构体
struct Car {
    string license;      // 车牌号
    int arrivalTime;     // 到达时间
    int parkingPosition; // 停车位置（从1开始）
};

// 全局变量
stack<Car> parkingLot;           // 停车场（栈）
queue<Car> waitingQueue;         // 等候队列（队列）
int parkingSpaces;               // 停车位数量
float costPerHour;               // 每小时费用

// 函数声明
void showMenu();
void carArrival();
void carDeparture();
void displayStatus();
void exitSystem();

// 显示菜单
void showMenu() {
    cout << "\n========== 停车场管理系统 ==========" << endl;
    cout << "1. 车辆到达" << endl;
    cout << "2. 车辆离开" << endl;
    cout << "3. 显示停车场状态" << endl;
    cout << "4. 退出系统" << endl;
    cout << "====================================" << endl;
    cout << "请选择操作(1-4): ";
}

// 车辆到达
void carArrival() {
    string license;
    int arrivalTime;
    
    cout << "\n--- 车辆到达 ---" << endl;
    cout << "请输入车牌号码: ";
    cin >> license;
    cout << "请输入到达时间(分钟): ";
    cin >> arrivalTime;
    
    Car newCar;
    newCar.license = license;
    newCar.arrivalTime = arrivalTime;
    
    // 检查停车场是否已满
    if (parkingLot.size() < parkingSpaces) {
        newCar.parkingPosition = parkingLot.size() + 1;
        parkingLot.push(newCar);
        cout << "✓ 车辆 " << license << " 已进入停车场，位置: " << newCar.parkingPosition << endl;
    } else {
        // 停车场已满，加入等候队列
        waitingQueue.push(newCar);
        cout << "✓ 停车场已满，车辆 " << license << " 已加入等候队列" << endl;
        cout << "  当前等候位置: " << waitingQueue.size() << endl;
    }
}

// 车辆离开
void carDeparture() {
    if (parkingLot.empty()) {
        cout << "\n✗ 停车场为空，没有车辆可以离开！" << endl;
        return;
    }
    
    int position;
    int departureTime;
    
    cout << "\n--- 车辆离开 ---" << endl;
    cout << "请输入车辆在停车场的位置(1-" << parkingLot.size() << "): ";
    cin >> position;
    
    if (position < 1 || position > parkingLot.size()) {
        cout << "✗ 位置不合法！" << endl;
        return;
    }
    
    cout << "请输入离开时间(分钟): ";
    cin >> departureTime;
    
    // 临时栈保存需要移出的车
    stack<Car> tempStack;
    Car departingCar;
    bool found = false;
    int currentPosition = parkingLot.size();
    
    // 找到要离开的车
    while (!parkingLot.empty()) {
        Car temp = parkingLot.top();
        parkingLot.pop();
        
        if (currentPosition == position) {
            departingCar = temp;
            found = true;
            break;
        }
        
        tempStack.push(temp);
        currentPosition--;
    }
    
    if (!found) {
        // 恢复栈
        while (!tempStack.empty()) {
            parkingLot.push(tempStack.top());
            tempStack.pop();
        }
        cout << "✗ 该位置没有车！" << endl;
        return;
    }
    
    // 处理在该车之后进入的车辆（需要先退出）
    cout << "\n需要退出的车辆列表(在 " << departingCar.license << " 之后进入):" << endl;
    int tempPosition = position + 1;
    while (!tempStack.empty()) {
        Car temp = tempStack.top();
        cout << "  位置 " << tempPosition << ": 车牌 " << temp.license << " 临时退出" << endl;
        tempPosition++;
        tempStack.pop();
    }
    
    // 计算停留时间和费用
    int parkingTime = departureTime - departingCar.arrivalTime;
    float cost = (parkingTime / 60.0f) * costPerHour;
    
    if (parkingTime < 0) parkingTime = 0;
    if (cost < 0) cost = 0;
    
    cout << "\n========== 离场信息 ==========" << endl;
    cout << "车牌号码: " << departingCar.license << endl;
    cout << "到达时间: " << departingCar.arrivalTime << " 分钟" << endl;
    cout << "离开时间: " << departureTime << " 分钟" << endl;
    cout << "停留时间: " << parkingTime << " 分钟 (" << fixed << setprecision(2) 
         << parkingTime / 60.0f << " 小时)" << endl;
    cout << "应缴费用: ¥" << fixed << setprecision(2) << cost << endl;
    cout << "==============================" << endl;
    
    cout << "\n✓ 车辆 " << departingCar.license << " 已离开停车场" << endl;
    
    // 将临时退出的车重新放回停车场
    if (!tempStack.empty()) {
        cout << "以下车辆按原次序重新进入停车场:" << endl;
        while (!tempStack.empty()) {
            Car temp = tempStack.top();
            temp.parkingPosition = parkingLot.size() + 1;
            parkingLot.push(temp);
            cout << "  车牌 " << temp.license << " 重新进入，位置: " << temp.parkingPosition << endl;
            tempStack.pop();
        }
    }
    
    // 检查等候队列，如果有车则进入停车场
    if (!waitingQueue.empty() && parkingLot.size() < parkingSpaces) {
        Car waitingCar = waitingQueue.front();
        waitingQueue.pop();
        waitingCar.parkingPosition = parkingLot.size() + 1;
        parkingLot.push(waitingCar);
        cout << "\n✓ 等候队列中的车辆 " << waitingCar.license 
             << " 已进入停车场，位置: " << waitingCar.parkingPosition << endl;
    }
}

// 显示停车场和等候队列状态
void displayStatus() {
    cout << "\n========== 停车场状态 ==========" << endl;
    cout << "停车位容量: " << parkingSpaces << " 辆" << endl;
    cout << "已停车数: " << parkingLot.size() << " 辆" << endl;
    cout << "等候车数: " << waitingQueue.size() << " 辆" << endl;
    cout << "每小时费用: ¥" << fixed << setprecision(2) << costPerHour << endl;
    cout << "================================" << endl;
    
    // 显示停车场内的车辆
    if (!parkingLot.empty()) {
        cout << "\n【停车场内的车辆】" << endl;
        stack<Car> temp = parkingLot;
        int position = temp.size();
        cout << "位置    车牌号    到达时间" << endl;
        cout << "---    ------    ------" << endl;
        while (!temp.empty()) {
            Car car = temp.top();
            cout << position << "       " << car.license << "      " 
                 << car.arrivalTime << " 分钟" << endl;
            temp.pop();
            position--;
        }
    } else {
        cout << "停车场内无车辆" << endl;
    }
    
    // 显示等候队列中的车辆
    if (!waitingQueue.empty()) {
        cout << "\n【等候队列中的车辆】" << endl;
        queue<Car> temp = waitingQueue;
        int queuePosition = 1;
        cout << "等候位置  车牌号    到达时间" << endl;
        cout << "-------  ------    ------" << endl;
        while (!temp.empty()) {
            Car car = temp.front();
            cout << "  " << queuePosition << "      " << car.license << "      " 
                 << car.arrivalTime << " 分钟" << endl;
            temp.pop();
            queuePosition++;
        }
    } else {
        cout << "等候队列为空" << endl;
    }
    cout << "\n================================" << endl;
}

// 退出系统
void exitSystem() {
    cout << "\n感谢使用停车场管理系统！再见！" << endl;
    exit(0);
}

// 系统初始化
void initializeSystem() {
    cout << "========== 停车场管理系统初始化 ==========" << endl;
    cout << "请输入停车位数量: ";
    cin >> parkingSpaces;
    cout << "请输入每小时费用(元): ";
    cin >> costPerHour;
    cout << "初始化完成！" << endl;
    cout << "========================================" << endl;
}

// 主函数
int main() {
    initializeSystem();
    
    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                carArrival();
                break;
            case 2:
                carDeparture();
                break;
            case 3:
                displayStatus();
                break;
            case 4:
                exitSystem();
                break;
            default:
                cout << "✗ 无效的选择，请重新输入！" << endl;
        }
    }
    
    return 0;
}

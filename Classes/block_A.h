#ifndef __BLOCK_A_H_
#define __BLOCK_A_H_

#include "cocos2d.h"
/*
 * This is the real game logic
 */
struct BB
{
public:
    BB() {color=cocos2d::Color3B::YELLOW;value=0;}
    cocos2d::Color3B color;
    int value;
};

class block_A
{
public:
    int x;//标志点横坐标
    int y;//标志点纵坐标
    int num;//种类标记
    int style;//形态标记
    int next_style;//下一个
    int next_num;//下一个
    cocos2d::Color3B color;//颜色
    cocos2d::Color3B next_color;//颜色
    bool isend;//标记是否到达最低层
    int m;//横坐标总数
    int n;//纵坐标总数
    bool temp_vaul;//有没有砖块
    int totalstyle_temp;
    int style_ini;
    bool isover;
    int speed;
    void randomNextProp();
    
public:
    block_A(int m_ori,int n_ori);//构造函数
    BB **MAP;
    BB **shadowMAP;
    BB **nextMAP;
    int M,N;
    void createBlk();//重新置为原值
    int totalstyle(int num1);//记录总的形态数
    int nextstyle(int style1);//下一种形态
    int prestyle(int style1);//前一种形态
    void setp(BB **MAP, int xori, int yori, int x2,int y2,cocos2d::Color3B color);//清零
    void clrp(BB **MAP, int xori, int yori, int x2,int y2,cocos2d::Color3B color);//置"1"
    void set(BB **MAP, int x1,int y1,int num1,int style1,cocos2d::Color3B color);//画砖块
    void clr(BB **MAP, int x1,int y1,int num1,int style1,cocos2d::Color3B color);//清除砖块
typedef void (block_A::*fun)(BB **MAP_, int xori, int yori, int x1, int y1, cocos2d::Color3B color);
    void doChange(fun func, BB **MAP_, int x1, int y1, int num, int style, cocos2d::Color3B color);
    bool existp(int xori, int yori, int x1,int y1);//点是否是实的
    bool exist(int x1,int y1,int num1,int style1);//是否存在东东
    void mvdown();//下移
    void mvleft();//左移
    void mvright();//右移
    void rotate();//顺时针转
    void rotate_re();//逆时针转
    void downtoend();//落至最低处
    int checkClear();//去掉需要消除的行并下落
    bool gameover();//判断是否游戏结束
    void newGame();
};

#endif //#define __BLOCK_A_H_

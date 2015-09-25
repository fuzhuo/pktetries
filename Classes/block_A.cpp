#include "block_A.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using std::cout;
using std::endl;

//======================================================================
block_A::block_A(int m_ori,int n_ori)
{
    m=m_ori;
    n=n_ori;
    isover=false;
    isend=false;
    speed=10;
    M=m_ori+2;//10+2
    N=n_ori+2;//20+2
    //init MAP
    MAP=(BB**)malloc(M*sizeof(BB*));
    for(int i=0; i<M; i++) {//N行，M列
        MAP[i]=static_cast<BB*>(new BB[N]);
    }
    for(int i1=0;i1<M;i1++){
        for(int j1=0;j1<N;j1++) {
            if(i1==M-1||j1==N-1||i1==0||j1==0) {
                MAP[i1][j1].value=1;
            } else MAP[i1][j1].value=0;
        }
    }
    //init shadowMAP
    shadowMAP=(BB**)malloc(M*sizeof(BB*));
    for(int i=0; i<M; i++) {//N行，M列
        shadowMAP[i]=static_cast<BB*>(new BB[N]);
    }
    for(int i1=0;i1<M;i1++){
        for(int j1=0;j1<N;j1++) {//clear everything
            if (j1%2 ==0) shadowMAP[i1][j1].value=1;
            shadowMAP[i1][j1].value=0;
        }
    }
    //init nextMAP
    nextMAP=(BB**)malloc(6*sizeof(BB*));
    for (int i=0; i<6; i++) {
        nextMAP[i]=static_cast<BB*>(new BB[6]);
    }
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            nextMAP[i][j].value=0;
        }
    }
    
    srand((unsigned)time(0));
    randomNextProp();
    //reset();
}
void block_A::randomNextProp()
{
    next_num=rand()%7+1;
    style_ini=totalstyle(next_num);
    next_style=rand()%style_ini+1;
    int co = rand()%6;
    if (co==0) next_color=cocos2d::Color3B::GREEN;
    else if (co==1) next_color=cocos2d::Color3B::YELLOW;
    else if (co==2) next_color=cocos2d::Color3B::BLUE;
    else if (co==3) next_color=cocos2d::Color3B::WHITE;
    else if (co==4) next_color=cocos2d::Color3B::ORANGE;
    else if (co==5) next_color=cocos2d::Color3B::MAGENTA;
}
void block_A::newGame()
{
    isover=false;
    isend=false;
    for(int i1=0;i1<M;i1++){
        for(int j1=0;j1<N;j1++) {
            if(i1==M-1||j1==N-1||i1==0||j1==0) {
                MAP[i1][j1].value=1;
            } else MAP[i1][j1].value=0;
        }
    }
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            nextMAP[i][j].value=0;
        }
    }
    randomNextProp();
    createBlk();
}
//--------------------------------------
void block_A::createBlk()
{
    x=m/2;y=2;
    num=next_num;
    style=next_style;
    color=next_color;
    //cout<<"newblk["<<x<<","<<y<<","<<next_num<<","<<next_style<<"]"<<endl;
    clr(nextMAP, 2, 2, next_num, next_style, next_color);
    randomNextProp();//随机化初始下一个块的形状，角度与颜色
    //cout<<"next newblk["<<x<<","<<y<<","<<next_num<<","<<next_style<<"]"<<endl;
    set(nextMAP, 2, 2, next_num, next_style, next_color);
    if (exist(x, y, num, style)) {
        cout<<"is over:"<<x<<","<<y<<endl;
        isover=true;
        isend=true;
        return;
    } else {
        isend=false;
        set(MAP, x,y,num,style,color);
    }
}
//-------------------------------------
bool block_A::gameover()
{
    if(isend==true&&exist(m/2,y=2,num,style))
    {isover=true; return true;}
    else
    {isover=false; return false;}
}
//--------------------------------------
void block_A::downtoend()
{
    while(isend==false)
        mvdown();
}
//--------------------------------------
int block_A::totalstyle(int num1)
{
    switch(num1)
    {
        case 1: totalstyle_temp=2; break;
        case 2: totalstyle_temp=1; break;
        case 3: totalstyle_temp=4; break;
        case 4: totalstyle_temp=4; break;
        case 5: totalstyle_temp=4; break;
        case 6: totalstyle_temp=2; break;
        case 7: totalstyle_temp=2; break;
    }
    return totalstyle_temp;
}
//--------------------------------------
int block_A::nextstyle(int style1)//下一种形态
{
    if(style1<totalstyle(num))
        style1++;
    else
        style1=1;
    return style1;
}
//--------------------------------------
int block_A::prestyle(int style1)//前一种形态
{
    if(style1>1)
        style1--;
    else
        style1=totalstyle(num);
    return style1;
}
//--------------------------------------
void block_A::setp(BB **MAP_, int xori, int yori, int x1=0,int y1=0,cocos2d::Color3B color_=cocos2d::Color3B::GREEN)
{                             
    MAP_[xori+x1][yori+y1].value=1;
    MAP_[xori+x1][yori+y1].color=color_;
}
void block_A::clrp(BB **MAP_, int xori, int yori, int x1=0,int y1=0,cocos2d::Color3B color_=cocos2d::Color3B::GREEN)
{                             
    MAP_[xori+x1][yori+y1].value=0;
    MAP_[xori+x1][yori+y1].color=color_;
}
void block_A::doChange(fun func, BB **MAP_, int x1, int y1, int num1, int style1, cocos2d::Color3B color)
{
    switch(num1)
    {
        case 1:
            switch(style1)
        {
            case 1:
            /*    *
             *   o*
             *   *
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,1,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);
                break;
            case 2:
            /*
             *  *o
             *   **
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,1,1,color);
                break;
        }
            break;
        case 2:
            switch(style1)
        {
            case 1:
            /*
             *   o*
             *   **
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,1,1,color);
                break;
        }
            break;
        case 3:
            switch(style1)
        {
            case 1:
            /*
             *  *o*
             *   *
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);
                break;
            case 2:
            /*
             *   *
             *   o*
             *   *
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,1,0,color);
                break;
            case 3:
            /*
             *   *
             *  *o*
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,0,1,color);
                break;
            case 4:
            /*   *
             *  *o
             *   *
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,-1,0,color);
                break;
        }
            break;
        case 4:
            switch(style1)
        {
            case 1:
            /*
             *  *o*
             *  *
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,-1,1,color);
                break;
            case 2:
            /*  **
             *   o
             *   *
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,-1,-1,color);
                break;
            case 3:
            /*
             *    *
             *  *o*
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,1,-1,color);
                break;
            case 4:
            /*   *
             *   o
             *   **
             */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,1,1,color);
                break;
        }
            break;
        case 5:
            switch(style1)
        {
            case 1: 
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,1,1,color);
                break;
            case 2: 
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,-1,1,color);
                break;
            case 3: 
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,-1,-1,color);
                break;
            case 4: 
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,1,-1,color);
                break;
        }
            break;
        case 6:
            switch(style1)
        {
            case 1:
                /*  *
                 *  *o
                 *   *
                 */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,-1,-1,color);
                break;
            case 2:
                /*
                 *   o*
                 *  **
                 */
                (this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,-1,1,color);
                break;
        }
            break;
        case 7:
            switch(style1)
        {
            case 1:
                /*    *
                 *    o
                 *    *
                 *    *
                 */
                (this->*func)(MAP_,x1,y1,0,-1,color);(this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,0,1,color);(this->*func)(MAP_,x1,y1,0,2,color);
                break;
            case 2:
                /*
                 *
                 *   *o***
                 *
                 */
                (this->*func)(MAP_,x1,y1,-1,0,color);(this->*func)(MAP_,x1,y1,0,0,color);(this->*func)(MAP_,x1,y1,1,0,color);(this->*func)(MAP_,x1,y1,2,0,color);
                break;
        }
            break;
    }
}
//-------------------------------------------------------------------------------------
void block_A::set(BB **MAP_, int x1,int y1,int num1, int style1, cocos2d::Color3B color)
{                                                  
    doChange(&block_A::setp, MAP_, x1, y1, num1, style1, color);
}

//------------------------------------------------------------------------------------
void block_A::clr(BB **MAP_, int x1,int y1,int num1,int style1,cocos2d::Color3B color)
{
    doChange(&block_A::clrp, MAP_, x1, y1, num1, style1, color);
}

//----------------------------------------------------------------------------

bool block_A::existp(int xori, int yori, int x1,int y1)
{
    if(MAP[xori+x1][yori+y1].value==1)
        return true;
    else
        return false;
}

//-------------------------------------------------

bool block_A::exist(int x1,int y1,int num1,int style1)
{
    //style=style1;
    switch(num1)
    {
        case 1:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,0)||existp(x1,y1,1,0)||existp(x1,y1,1,-1)||existp(x1,y1,0,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 2: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,0,1)||existp(x1,y1,1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
        case 2:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,1)||existp(x1,y1,1,0)||existp(x1,y1,1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
        case 3:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,1,0)||existp(x1,y1,0,-1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 2: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,-1)||existp(x1,y1,0,1)||existp(x1,y1,1,0))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 3: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,1,0)||existp(x1,y1,0,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 4: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,-1)||existp(x1,y1,0,1)||existp(x1,y1,-1,0))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
        case 4:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,1,0)||existp(x1,y1,-1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 2: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,-1)||existp(x1,y1,0,1)||existp(x1,y1,-1,-1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 3: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,1,0)||existp(x1,y1,1,-1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 4: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,-1)||existp(x1,y1,0,1)||existp(x1,y1,1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
        case 5:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,1,0)||existp(x1,y1,1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 2: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,-1)||existp(x1,y1,0,1)||existp(x1,y1,-1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 3: 
                if(existp(x1,y1,0,0)||existp(x1,y1,-1,0)||existp(x1,y1,1,0)||existp(x1,y1,-1,-1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 4: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,-1)||existp(x1,y1,0,1)||existp(x1,y1,1,-1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
        case 6:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,0)||existp(x1,y1,0,1)||existp(x1,y1,-1,0)||existp(x1,y1,-1,-1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 2: 
                if(existp(x1,y1,0,0)||existp(x1,y1,1,0)||existp(x1,y1,0,1)||existp(x1,y1,-1,1))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
        case 7:
            switch(style1)
        {
            case 1: 
                if(existp(x1,y1,0,-1)||existp(x1,y1,0,0)||existp(x1,y1,0,1)||existp(x1,y1,0,2))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
            case 2: 
                if(existp(x1,y1,-1,0)||existp(x1,y1,0,0)||existp(x1,y1,1,0)||existp(x1,y1,2,0))
                    temp_vaul =true;
                else
                    temp_vaul =false;
                break;
        }
            break;
    }
    return temp_vaul;
}
//----------------------------------------------------------------------------------
void block_A::rotate()
{
    if(isover==false && isend==false && speed!=0)
    {
        clr(MAP, x,y,num,style,color);
        if(exist(x,y,num,nextstyle(style))==false) {
            style=nextstyle(style);
            set(MAP, x,y,num,style,color);
        } else if(exist(x-1,y,num,style)==false) {
            x-=1;
            style=nextstyle(style);
            set(MAP, x,y,num,style,color);
        } else if(exist(x+2,y,num,style)==false) {
            x+=2;
            style=nextstyle(style);
            set(MAP, x,y,num,style,color);
        } else if(exist(x-1,y-1,num,style)==false) {
            x-=1;y-=1;
            style=nextstyle(style);
            set(MAP, x,y,num,style,color);
        } else {
            y+=1;
            style=prestyle(style);
            set(MAP, x,y,num,prestyle(style),color);
        }
    }
}
void block_A::rotate_re()
{
    if(isover==false && isend==false &&speed!=0)
    {               
        clr(MAP, x,y,num,style,color);
        if(exist(x,y,num,prestyle(style))==false) {
            style=prestyle(style);
            set(MAP, x,y,num,style,color);
        } else if(exist(x-1,y,num,style)==false) {
            x-=1;
            style=prestyle(style);
            set(MAP, x,y,num,style,color);
        } else if(exist(x+2,y,num,style)==false) {
            x+=2;
            style=prestyle(style);
            set(MAP, x,y,num,style,color);
        } else if(exist(x-1,y-1,num,style)==false) {
            x-=1;y-=1;
            style=prestyle(style);
            set(MAP, x,y,num,style,color);
        } else {
            y+=1;
            style=nextstyle(style);
            set(MAP, x,y+1,num,nextstyle(style),color);
        }
    }
}
void block_A::mvleft()
{
    if(isover==false && isend==false &&speed!=0)
    {               
        clr(MAP, x,y,num,style,color);
        if(exist(x-1,y,num,style)==false) {
            x-=1;
            set(MAP, x,y,num,style,color);
        } else {
            set(MAP, x,y,num,style,color);
        }
    }
}
void block_A::mvright()
{
    if(isover==false && isend==false &&speed!=0)
    {               
        clr(MAP, x,y,num,style,color);
        if(!exist(x+1,y,num,style)) {
            x+=1;
            set(MAP, x,y,num,style,color);
        } else {
            set(MAP, x,y,num,style,color);
        }
    }
}

void block_A::mvdown()
{
    if(isover==false && isend==false &&speed!=0)
    {               
        clr(MAP, x,y,num,style,color);
        if(!exist(x,y+1,num,style)) {
            y+=1;
            set(MAP, x,y,num,style,color);
        } else {
            set(MAP, x,y,num,style,color);
            isend=true;
        }
    }             
}

int block_A::checkClear()
{
    int templine=0;
    if(isend==true)
    {
        for(int nn=N-2;nn>=1;nn--)//列标
        {
            bool line_full=true;
            for(int mm=1;mm<=M-2;mm++)//行标
            {
                if(MAP[mm][nn].value==0) {
                    line_full=false;
                    break;
                }
            }
            if (line_full)
            {
                templine++;
                for(int jj=nn;jj>1;jj--)//列标
                    for(int mm=1;mm<=M-2;mm++)//行标
                    {
                        MAP[mm][jj]=MAP[mm][jj-1];//下移一位
                    }
                nn++;
            }
        }
    }
    return templine;
}

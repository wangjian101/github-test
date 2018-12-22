#ifndef H_BinaryExpTree_H
#define H_BinaryExpTree_H

//#include "二叉树抽象数据类型的实现（二叉链表）.h"
#define stacksize 50

//二叉链表结点结构
struct BTNode {
    TElemType data;        //结点值
    BTNode *lchild;    //左孩子结点指针
    BTNode *rchild;    //右孩子结点指针
};

//二叉链表初始化
void InitBinaryTree( BTNode* &T )
{
    T = NULL;
}

//销毁二叉链表形式的二叉树T
void DestroyBinaryTree( BTNode* &T )
{
    if ( T )
    {
        DestroyBinaryTree ( T->lchild );
        DestroyBinaryTree ( T->rchild );
        delete T;
    }
    T = NULL;
}

// 判断c是否为运算符
bool JudgeSimbol(char &c)
{
    switch ( c )
    {
        case'~':
        case'&':
        case'|':
        case'(':
        case')':
        case'#':return true;
        default:return false;
    }
}

// 判断两运算符t1,t2的优先关系
char Precede( char &t1, char &t2 )
{
    char f;
    switch ( t2 )
    {
         case '|':
             if( t1 == '(')
                 f = '<';
             else
                 f = '>';
             break;
         case '&':
             if(t1 == '|' || t1 == '(')
                f = '<';
             else
                f = '>';
             break;
         case '~':
             if ( t1 == '|' || t1 == '&' || t1 == '(' )
                 f = '<';
             else
                 f = '>';
             break;
         case '(':
             f = '<';
             break;
         case ')':
             if ( t1 == '(' )
                 f = '=';
             else
                 f = '>';
             break;
         case '#':
                 f = '>';
             break;
    }
    return f;
}

typedef BTNode* SElemType;
#include "SqStack.h"

//利用操作数OPND工作栈及运算符工作栈OPTR建立表达式二叉树
bool CreateBinaryExpTree( BTNode* &T, char ch[], BTNode* leaf[] )//记录已有的操作数位置
{
    SqStack OPND, OPTR;
    InitSqStack( OPND, stacksize );
    InitSqStack( OPTR, stacksize );
    BTNode *p, *q, *s;
    int i = 0;
    char theta;

    while ( ! SqStackEmpty( OPTR ) || ch[ i ] != '#' ) //结束条件是OPTR栈为空且字符为'#'
    {
        if ( ch[ i ] >= 'A' && ch[ i ] <= 'Z') //操作数不确定且为大写字母
        {
            if(!leaf[ ch[ i ]-'A' ]){//操作数未重复
                p = new BTNode;
                p->data = ch[ i ];
                p->lchild = p->rchild = NULL;
                leaf[ ch[ i ]-'A' ] = p;
            }
            else{//操作数重复
                p = leaf[ ch[ i ]-'A' ];
            }
            PushSqStack( OPND, p );
            i++;
        }
        else if (ch[i] >='0' && ch[i] <='1'){
            if(ch[i] == '0'){
                p = new BTNode;
                p->data = 0;
                p->lchild = p->rchild = NULL;
            }
            else{
                p = new BTNode;
                p->data = 1;
                p->lchild = p->rchild = NULL;
            }
            PushSqStack(OPND, p);
            i++;
        }
        else if ( JudgeSimbol( ch[ i ] ) )//处理运算符
        {
            if ( SqStackEmpty( OPTR ) )//当前运算符在工作栈OPTR为空时建子树入栈
            {
                q = new BTNode;
                q->data = ch[ i ];
                q->lchild = q->rchild = NULL;
                PushSqStack( OPTR, q );
                i++;
            }
            else if ( GetTop( OPTR, p ) )
            {
                theta = p->data;
                switch ( Precede( theta, ch[ i ] ) )
                {
                    case'<': // 栈顶元素优先权低,当前运算符子树进栈
                        q = new BTNode;
                        q->data = ch[i];
                        q->lchild = q->rchild = NULL;
                        PushSqStack( OPTR, q );
                        i++;
                        break;
                    case'=':// 脱括号
                        PopSqStack( OPTR, s );
                        delete s;
                        i++;
                        break;
                    case'>':// 退栈并重组表达式树入栈
                        PopSqStack( OPTR, s );
                        if ( SqStackEmpty( OPND ) )
                            return false;
                        PopSqStack( OPND, p );
                        s->rchild = p;
                        if(s->data != '~'){//非单目运算
                            if ( SqStackEmpty( OPND ) )
                                return false;
                            PopSqStack( OPND, q );
                            s->lchild = q;
                        }
                        PushSqStack( OPND, s );
                }
            }
        }
        else
            i++;
    }
    if( PopSqStack( OPND, T ))
        return true;
    else
        return false;
}

//字符theta决定a与b 执行何种运算
int Opreate( int a, char theta, int b )
{
    int c;
    switch( theta )
    {
        case'|':
            c = a || b;
            break;
        case'&':
            c = a && b;
            break;
        case'~':
            c = !b;
            break;
    }
    return c;
}

//用后序遍历的方式对用二叉表达式树表示的表达式求值
int Evaluate( BTNode* &T )
{
    if ( T )
    {
        if ( ! T->lchild && ! T->rchild )
            return T->data;
        return Opreate( Evaluate( T->lchild ), T->data, Evaluate( T->rchild ) );
    }
    return -1;
}

//先序递归遍历二叉树
void PreorderTraverse( BTNode* &T, void( *visit )(BTNode* &T, TElemType &e ) )
{
    if ( T )
    {
        if ( T->data )
            visit(T, T->data );
        PreorderTraverse( T->lchild, visit );
        PreorderTraverse( T->rchild, visit );
    }
}
//中序递归遍历二叉树
void InorderTraverse( BTNode* &T, void( *visit )(BTNode* &T, TElemType &e ) )
{
    if ( T )
    {
        InorderTraverse( T->lchild, visit );
        if ( T->data )
            visit(T, T->data );
        InorderTraverse( T->rchild, visit );
    }
}
//后序递归遍历二叉树
void PostorderTraverse( BTNode* &T, void( *visit )(BTNode* &T, TElemType &e ) )
{
    if ( T )
    {
        PostorderTraverse( T->lchild, visit );
        PostorderTraverse( T->rchild, visit );
        if ( T->data )
            visit(T, T->data );
    }
}

#endif


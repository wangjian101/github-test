#ifndef H_BinaryExpTree_H
#define H_BinaryExpTree_H

//#include "�����������������͵�ʵ�֣���������.h"
#define stacksize 50

//����������ṹ
struct BTNode {
    TElemType data;        //���ֵ
    BTNode *lchild;    //���ӽ��ָ��
    BTNode *rchild;    //�Һ��ӽ��ָ��
};

//���������ʼ��
void InitBinaryTree( BTNode* &T )
{
    T = NULL;
}

//���ٶ���������ʽ�Ķ�����T
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

// �ж�c�Ƿ�Ϊ�����
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

// �ж��������t1,t2�����ȹ�ϵ
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

//���ò�����OPND����ջ�����������ջOPTR�������ʽ������
bool CreateBinaryExpTree( BTNode* &T, char ch[], BTNode* leaf[] )//��¼���еĲ�����λ��
{
    SqStack OPND, OPTR;
    InitSqStack( OPND, stacksize );
    InitSqStack( OPTR, stacksize );
    BTNode *p, *q, *s;
    int i = 0;
    char theta;

    while ( ! SqStackEmpty( OPTR ) || ch[ i ] != '#' ) //����������OPTRջΪ�����ַ�Ϊ'#'
    {
        if ( ch[ i ] >= 'A' && ch[ i ] <= 'Z') //��������ȷ����Ϊ��д��ĸ
        {
            if(!leaf[ ch[ i ]-'A' ]){//������δ�ظ�
                p = new BTNode;
                p->data = ch[ i ];
                p->lchild = p->rchild = NULL;
                leaf[ ch[ i ]-'A' ] = p;
            }
            else{//�������ظ�
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
        else if ( JudgeSimbol( ch[ i ] ) )//���������
        {
            if ( SqStackEmpty( OPTR ) )//��ǰ������ڹ���ջOPTRΪ��ʱ��������ջ
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
                    case'<': // ջ��Ԫ������Ȩ��,��ǰ�����������ջ
                        q = new BTNode;
                        q->data = ch[i];
                        q->lchild = q->rchild = NULL;
                        PushSqStack( OPTR, q );
                        i++;
                        break;
                    case'=':// ������
                        PopSqStack( OPTR, s );
                        delete s;
                        i++;
                        break;
                    case'>':// ��ջ��������ʽ����ջ
                        PopSqStack( OPTR, s );
                        if ( SqStackEmpty( OPND ) )
                            return false;
                        PopSqStack( OPND, p );
                        s->rchild = p;
                        if(s->data != '~'){//�ǵ�Ŀ����
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

//�ַ�theta����a��b ִ�к�������
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

//�ú�������ķ�ʽ���ö�����ʽ����ʾ�ı��ʽ��ֵ
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

//����ݹ����������
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
//����ݹ����������
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
//����ݹ����������
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


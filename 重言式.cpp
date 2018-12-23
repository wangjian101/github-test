#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef int TElemType;
#include "逻辑运算树（二叉链表）.h"


bool Tautology(BTNode* leaf[], int* leafloc, int leafnum, BTNode* &T){
    int flag=0;
    if(leafnum){
        for(int i=0;i<(int)pow(2,leafnum);i++){
            int temp=i;
            for(int j=0;j<leafnum;j++){
                leaf[leafloc[j]]->data=temp%2;
                temp/=2;
            }
            flag+=Evaluate(T);//后序遍历求值
            if(flag!=0 && flag!=i+1){
                cout<<"Satisfactible"<<endl;
                return 0;
            }
        }
        if(flag){
            cout<<"Ture forever"<<endl;
            return 1;
        }
        else{
            cout<<"False forever"<<endl;
            return 1;
        }
    }
    else{
        if(Evaluate(T))
            cout<<"Ture forever"<<endl;
        else
            cout<<"Flase forever"<<endl;
    }
    return 0;
}


void truth_table(BTNode* leaf[], int* leafloc, int leafnum, BTNode* &T){
    for(int i=0;i<leafnum;i++){
        cout<<(char)('A'+leafloc[i])<<'\t';
        if(i+1==leafnum)
            cout<<"|"<<"Result"<<endl;
        else
            cout<<"|";
    }
    for(int i=0;i<(int)pow(2,leafnum);i++){
        int temp=i;
        for(int j=0;j<leafnum;j++){
            leaf[leafloc[j]]->data=temp%2;
            cout<<temp%2<<'\t';
            if(j+1!=leafnum){
                cout<<"|";
                temp/=2;
            }
        }
        cout<<"|"<<Evaluate(T)<<endl;//后序遍历求值
    }
}


int main()
{
    BTNode* T;
    char ch[100];
    char yes;
    BTNode* leaf[26]={0};//保存'A'-'Z'未知数
    int leafnum=0;//叶子节点数目
    int leafloc[26];//叶子节点在leaf数组中位置
    InitBinaryTree(T);
    cout<<"《重言式判别》"<<endl<<endl;
    cout<<"请输入待判别逻辑表达式，未知数以大写字母表示(A-Z)，"<<endl;
    cout<<"与：&"<<endl<<"或：|"<<endl<<"非：~"<<endl;
    cout<<"并以“#”结尾："<<endl;
    cin>>ch;
    CreateBinaryExpTree(T, ch, leaf);
    for(int i=0;i<26;i++)
        if(leaf[i]){
            leafloc[leafnum]=i;
            leafnum++;
        }
    cout<<"结果:"<<endl;
    Tautology(leaf, leafloc, leafnum, T);
    cout<<"是否显示真值表（是:Y）"<<endl;
    if(cin>>yes && yes=='Y')
        truth_table(leaf, leafloc, leafnum, T);
    DestroyBinaryTree(T);
    system("pause");
    return 0;
}

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef int TElemType;
#include "�߼�����������������.h"


bool Tautology(BTNode* leaf[], int* leafloc, int leafnum, BTNode* &T){
    int flag=0;
    if(leafnum){
        for(int i=0;i<(int)pow(2,leafnum);i++){
            int temp=i;
            for(int j=0;j<leafnum;j++){
                leaf[leafloc[j]]->data=temp%2;
                temp/=2;
            }
            flag+=Evaluate(T);//���������ֵ
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
        cout<<"|"<<Evaluate(T)<<endl;//���������ֵ
    }
}


int main()
{
    BTNode* T;
    char ch[100];
    char yes;
    BTNode* leaf[26]={0};//����'A'-'Z'δ֪��
    int leafnum=0;//Ҷ�ӽڵ���Ŀ
    int leafloc[26];//Ҷ�ӽڵ���leaf������λ��
    InitBinaryTree(T);
    cout<<"������ʽ�б�"<<endl<<endl;
    cout<<"��������б��߼����ʽ��δ֪���Դ�д��ĸ��ʾ(A-Z)��"<<endl;
    cout<<"�룺&"<<endl<<"��|"<<endl<<"�ǣ�~"<<endl;
    cout<<"���ԡ�#����β��"<<endl;
    cin>>ch;
    CreateBinaryExpTree(T, ch, leaf);
    for(int i=0;i<26;i++)
        if(leaf[i]){
            leafloc[leafnum]=i;
            leafnum++;
        }
    cout<<"���:"<<endl;
    Tautology(leaf, leafloc, leafnum, T);
    cout<<"�Ƿ���ʾ��ֵ����:Y��"<<endl;
    if(cin>>yes && yes=='Y')
        truth_table(leaf, leafloc, leafnum, T);
    DestroyBinaryTree(T);
    system("pause");
    return 0;
}

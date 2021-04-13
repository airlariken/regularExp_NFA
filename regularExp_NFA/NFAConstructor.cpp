//
//  NFAConstructor.cpp
//  regularExp_NFA
//
//  Created by 陈梓玮 on 2021/4/7.
//

#include "NFAConstructor.hpp"

int NFAConstrctor::operatorPriority(const char& t)
{

    if(t == '*')
    {
        return 3;
    }
        
    if(t == '-')//连接运算符
    {
        return 2;
    }
        
    if(t == '|')
    {
        return 1;
    }
    
    if(t == '(')
    {
        return 0;
    }
    else{
        cerr<<"there is no such char!"<<endl;
        return -1;
    }

}
bool NFAConstrctor::getCh()
{
    if (stringpointer < regular_exp.length()) {
        t = regular_exp[stringpointer];
        stringpointer++;
        return 1;
    }
    else
        return 0;

}
void NFAConstrctor::transformToSuffix()
{
    while (getCh() == true) {
        if (t == '(')
            operator_stk.push(t);
        else if (t =='*' || t == '-' || t == '|') {
            while (1)
            {
                int p1 = operatorPriority(t);
                if (operator_stk.empty()) {
                    operator_stk.push(t);
                    break;
                }
                int p2 = operatorPriority(operator_stk.top());
                if (p1 > p2) {
                    operator_stk.push(t);
                    break;
                }
                else{
                    suffix_exp += operator_stk.top();
                    operator_stk.pop();
                }
            }
        }
        else if(t == ')') {
            while (!operator_stk.empty()) {
                if (operator_stk.top() == '(') {
                    operator_stk.pop();
                    break;
                }
                suffix_exp += operator_stk.top();    operator_stk.pop();
                
            }
        }
        else{
            suffix_exp += t;
        }
    }
    while (!operator_stk.empty()) {
        suffix_exp+=operator_stk.top();
        operator_stk.pop();
    }
}


bool NFAConstrctor::get_suffix_char()
{
    if (suffix_pointer < suffix_exp.length()) {
        suffix_char = suffix_exp[suffix_pointer];
        suffix_pointer++;
        return 1;
    }
    else
        return 0;
}

void NFAConstrctor::createSingleNFA()//遇到符号建立单个NFA
{
    NFANode *first = new NFANode(NFA_num_cnt,NFA_num_cnt+1,suffix_char);
    NFANode *second = new NFANode(NFA_num_cnt+1);
    node_set.push_back(first);
    node_set.push_back(second);
    NFA_num_cnt+=2;
    NFAEdge *newEdge = new NFAEdge(first, second);
    NFA_stk.push(newEdge);
    
}
void NFAConstrctor::createNFA()
{

    while (get_suffix_char()==true) {
        if (suffix_char <= 'z' &&suffix_char >= 'a') {
            terminal_symbol.push_back(suffix_char);//放入终结符集合中
            createSingleNFA();
            continue;
        }
        else if(suffix_char == '*'){            //处理闭包
            NFANode *first = new NFANode(NFA_num_cnt++);
            NFANode *second = new NFANode(NFA_num_cnt++);
            node_set.push_back(first);
            node_set.push_back(second);
            first->e_closure_set.push_back(second->num);    //用e闭包连接新NFA的两个节点
            NFAEdge *top = NFA_stk.top();   NFA_stk.pop();
            first->e_closure_set.push_back(top->head->num); //新NFA头结点用e闭包连接栈顶NFA头结点
            top->next->e_closure_set.push_back(first->num); //栈顶尾结点用e闭包连接新NFA头结点
            top->next->e_closure_set.push_back(second->num);//栈顶尾结点用e闭包连接新NFA尾结点
            NFAEdge *newEdge = new NFAEdge(first, second);
            NFA_stk.push(newEdge);
        }
        else if(suffix_char == '|'){            //处理闭包
            NFANode *first = new NFANode(NFA_num_cnt++);
            NFANode *second = new NFANode(NFA_num_cnt++);
            node_set.push_back(first);
            node_set.push_back(second);
            NFAEdge *top1 = NFA_stk.top();   NFA_stk.pop();
            NFAEdge *top2 = NFA_stk.top();   NFA_stk.pop();
            first->e_closure_set.push_back(top1->head->num);
            first->e_closure_set.push_back(top2->head->num);
            top1->next->e_closure_set.push_back(second->num);
            top2->next->e_closure_set.push_back(second->num);
            NFAEdge *newEdge = new NFAEdge(first, second);
            NFA_stk.push(newEdge);
        }
        else if(suffix_char == '-'){            //处理闭包
            NFAEdge *top1 = NFA_stk.top();   NFA_stk.pop();
            NFAEdge *top2 = NFA_stk.top();   NFA_stk.pop();
//            top1->next->e_closure_set.push_back(top2->head->num);
            top2->next->e_closure_set.push_back(top1->head->num);
            NFA_stk.push(top1);
            
            
//            NFA n1, n2, n;                /*定义一个新的NFA n*/
//                    n2 = NfaStack.top();                /*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
//                    NfaStack.pop();
//                    n1 = NfaStack.top();
//                    NfaStack.pop();
//                    add(n1.tail, n2.head);                /*n1的尾通过ε指向n2的尾*/
//                    n.head = n1.head;                    /*n的头为n1的头*/
//                    n.tail = n2.tail;                    /*n的尾为n2的尾*/
//                    NfaStack.push(n);                    /*最后将新生成的NFA入栈*/
        }
    }
}
void NFAConstrctor::output_NFA()
{
    for (int i = 0; i < node_set.size(); ++i) {
        cout<<node_set[i]->num<<'\t'<<node_set[i]->trans_char<<'\t'<<node_set[i]->next_num<<endl;
    }
}
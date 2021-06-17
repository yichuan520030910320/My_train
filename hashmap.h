//
// Created by 18303 on 2021/6/1.
//

#ifndef BPLUSTREE_HASHMAP_H
#define BPLUSTREE_HASHMAP_H
using namespace std;
#include <functional>
template<class nametype, class datatype>
class hashmap {
public: std::hash<nametype> h;
    class node {
    public:
        nametype data;
        datatype info;
        node *nxt = nullptr;
        node *pre = nullptr;
        node() = delete;
        node(nametype a, datatype b, node *temp = nullptr) : data(a), info(b), nxt(temp) {
        }
        node(const node &other) {
            nxt = other.nxt;
            pre = other.pre;
            data = other.data;
            info = other.info;
        }
    };
    class list {
    public: node *headnode= nullptr;
        int nodenum = 0;
        ~list() {
            node *tempp;
            for (int i = 1; i <= nodenum; ++i) {
                tempp = headnode;
                headnode = headnode->nxt;
                delete tempp;
            }
            nodenum = 0;
        }
        void insert(nametype a, datatype b) {
            nodenum++;
            headnode = new node(a, b, headnode);
        }
        bool erase(nametype &tempdata) {
            if (nodenum == 0) return false;
            if (headnode->data == tempdata) {
                node *del = headnode;
                headnode = headnode->nxt;
                nodenum--;
                delete del;
                return true;
            }
            node *temp = headnode;
            node *temp2 = headnode->nxt;
            while (temp2 != nullptr) {

                if (temp2->data == tempdata) {
                    node *del = temp2;
                    temp->nxt = temp2->nxt;
                    delete del;
                    nodenum--;
                    return true;
                }
                temp2 = temp2->nxt;
                temp = temp->nxt;
            }
            return false;
        }

        node *find(nametype &tempdata) {
            if (nodenum==0) return nullptr;
            if (headnode->data == tempdata) {
                return headnode;
            }
            node *temp = headnode;
            node *temp2 = headnode->nxt;
            while (temp2 != nullptr) {
                if (temp2->data == tempdata) {

                    return temp2;
                }
                temp2 = temp2->nxt;
                temp = temp->nxt;
            }
            return nullptr;
        }
    };
    list *headlist= nullptr;
    long long totalnum=0;
public:
   explicit hashmap(long long num){
       totalnum=num;
        headlist =new list[num];
    }
    ~ hashmap(){
        delete[]headlist;
    }
    void insert(nametype a, datatype b){
        long long  pos=h(a)%totalnum;
        headlist[pos].insert(a,b);
    }
bool erase(nametype a){
    long long  pos=h(a)%totalnum;
   return headlist[pos].erase(a);
    }
    std::pair<bool,datatype> find(nametype a){
        long long  pos=h(a)%totalnum;
        node * ans=headlist[pos].find(a);
        if (ans== nullptr) return  std::make_pair(false,datatype());
        return  std::make_pair(true,ans->info);
    }
    void clear(){
        delete[]headlist;
        headlist =new list[totalnum];
    }
};
#endif //BPLUSTREE_HASHMAP_H

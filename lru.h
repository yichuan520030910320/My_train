//
// Created by 18303 on 2021/6/1.
//

#ifndef BPLUSTREE_LRU_H
#define BPLUSTREE_LRU_H

#include "hashmap.h"

using namespace std;

#include <iostream>
#include <fstream>

template<typename infotype, int MAXSIZE = 500>

class Diskmanager {
public:
    class lrulist_node {
        lrulist_node() = default;

    public:
        lrulist_node *pre;
        lrulist_node *nxt;
    public:
        infotype data;
    public:
        bool ismodified = false;
        int address = -1;

        lrulist_node(lrulist_node *_pre, lrulist_node *after, int ad, infotype data_) : pre(_pre), nxt(after),
                                                                                        data(data_),
                                                                                        address(ad) {

        }

        lrulist_node(lrulist_node &other) {
            pre = other.pre, nxt = other.nxt, data = other.data, ismodified = other.ismodified, address = other.address;
        }
        // ~lrulist_node()=default;

    };

    using map_back = typename std::pair<bool, lrulist_node *>;
public:
    lrulist_node *head = nullptr;
    lrulist_node *tail = nullptr;//这里的tail和head都是有实际内容的
    long long cachenum = 0;
    hashmap<int, typename Diskmanager::lrulist_node *> map_hash;

    bool find(int address) {
        if (map_hash.find(address).first) {//可优化一次fin即可
            lrulist_node *ans = map_hash.find(address).second;
            if (ans->pre) {
                lrulist_node *temppre = ans->pre;
                lrulist_node *tempnxt = ans->nxt;

                head->pre=ans;
                ans->nxt=head;
                ans->pre= nullptr;
                head = ans;

                if (tempnxt) tempnxt->pre = temppre, temppre->nxt = tempnxt;
                else { temppre->nxt = nullptr, tail = temppre; }
            }
            return true;
        }
        return false;
    }

    pair<int, infotype> pop() {

        pair<int, infotype> returnans;
        returnans.first = tail->address;
        returnans.second = tail->data;
        if (!tail->ismodified) returnans.first = -1;//若不变则不对文件作出修改
        map_hash.erase(tail->address);

        lrulist_node *del = tail;
        lrulist_node *pretemp = tail->pre;
        if (pretemp) tail = pretemp, pretemp->nxt = nullptr;
        else head = tail = nullptr;
        delete del;
        del = nullptr;
        cachenum--;
        return returnans;
    }

    void push(int addrsee, infotype data) {
        if (cachenum == 0) {
            head = new lrulist_node(nullptr, nullptr, addrsee, data);
            tail = head;
            cachenum = 1;
        } else {
            ++cachenum;
            lrulist_node *temp = new lrulist_node(nullptr, head, addrsee, data);
            head->pre = temp;
            head = temp;
        }
        map_hash.insert(addrsee, head);
        if (cachenum == MAXSIZE) {
            pair<int, infotype> ans(pop());
            if (ans.first != -1) {
                file.seekg(ans.first);
                file.write(reinterpret_cast<char *>(&ans.second), sizeof(infotype));
            }
        }
    }


public:
    fstream file;
    string name;

public:
    Diskmanager();

    Diskmanager(const string &temp) : map_hash(997) {
        name = temp;
        file.close();
        file.open(temp, std::ios_base::binary | std::ios::in | std::ios::out);
        if (!file) { file.open(temp, std::ios::out | std::ios::binary); }
    }

    ~Diskmanager() {
        //写入未被修改的东西

        //文件操作！！
     file.close();
       file.open(name, std::ios_base::binary | std::ios::in | std::ios::out);
        if (!file) { file.open(name, std::ios::out | std::ios::binary); }

        lrulist_node *ptr = head;
        while (ptr) {
           // cout<<ptr->ismodified<<"&&&&&&&& flag"<<endl;
           // cout<<*(ptr->data)<<endl;

            if (ptr->ismodified) {
                file.seekg(ptr->address);
                file.write(reinterpret_cast<char *>(&ptr->data), sizeof(infotype));
            }
            lrulist_node *del = ptr;
            ptr = ptr->nxt;
            delete del;
        }
        head = tail = nullptr;
        file.close();
    }

public:
    void read(int infoaddrss, infotype &info) {
        if (find(infoaddrss)) {
            map_back aaaans = map_hash.find(infoaddrss);
            lrulist_node *tmp = aaaans.second;
            info = tmp->data;
        } else {
            file.seekg(infoaddrss);
            file.read(reinterpret_cast<char *>(&info), sizeof(info));
            push(infoaddrss, info);
        }
        // cout<<cachenum<<"*******"<<endl;
    }


public:
//    void show(){
//        std::cout << " Cache List show : ------" << std::endl;
//        lrulist_node * tmp = head;
//        while(tmp != nullptr){
//            tmp->data.showleaf(tmp->data);
//            tmp = tmp->nxt;
//        }
//        std::cout << "---- over -----" << std::endl;
//    }
    void write(int infoaddress, infotype &info) {
//反复读不代表会反复写,所以不用丢入双向链表中
        map_back aaaans = map_hash.find(infoaddress);
        lrulist_node *tmp(aaaans.second);
        if (aaaans.first) {

            if (tmp->data != info) {
                tmp->data = info;
                tmp->ismodified = true;
            }

        } else {
            file.seekg(infoaddress);
            file.write(reinterpret_cast<char *>(&info), sizeof(info));
        }

    }
};

#endif //BPLUSTREE_LRU_H

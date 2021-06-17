//
// Created by 18303 on 2021/4/30.
//

#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include "utility.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <fstream>
#include "lru.h"

#define debug

//根据磁盘大小调整，在debug lru之后调整！
struct mystring {
    char mychar[120];

    mystring() = default;

    mystring(char a[120]) {
        strcpy(mychar, a);
    }

    mystring(const mystring &c) {
        strcpy(mychar, c.mychar);
    }

    mystring &operator=(const mystring &other) {
        if (this == &other) return *this;
        strcpy(mychar, other.mychar);
        return *this;
    }

    bool operator<(const mystring c) {
        return strcmp(mychar, c.mychar) < 0;
    }

    bool operator>(const mystring c) {
        return strcmp(mychar, c.mychar) > 0;
    }

    friend bool operator==(const mystring &a1, const mystring &a2);

    friend bool operator!=(const mystring &a1, const mystring &a2);

    ~mystring() = default;
};

inline bool operator!=(const mystring &a1, const mystring &a2) {
    return strcmp(a1.mychar, a2.mychar) != 0;
}

inline bool operator==(const mystring &a1, const mystring &a2) {
    return strcmp(a1.mychar, a2.mychar) == 0;
}

namespace sjtu {
    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    //std::less默认是<,key的排序方式由外部传入的变量决定
#define MAXNUM 5
#define MINNUM 2

    class BPtree {
    public:
        typedef std::pair<Key, T> value_type;

        Compare cmp;
        const int MAX = ((4096 - sizeof(int) * 4 - sizeof(bool)) / sizeof(Key) + sizeof(T) - 1) % 2 == 0 ?
                        (4096 - sizeof(int) * 4 - sizeof(bool)) / sizeof(Key) + sizeof(T) - 2 :
                        (4096 - sizeof(int) * 4 - sizeof(bool)) / sizeof(Key) + sizeof(T) - 1;
        const int MIN = MAX / 2;

    public:

        class node {
        public:
            int fa = -1;
            int size = 0;
            int address[MAXNUM + 5];//节点内部的数据(记录地址)
            int pre, nxt;
            bool is_leaf = false;
            Key info[MAXNUM + 5];//节点内部的数据(真正的数据存储)

            node() {
                for (int i = 0; i < MAXNUM + 3; ++i) {
                    address[i] = -1;
                }
            };

            node(int fa_, int size_, int pre_, int nxt_, bool ifleaf) : fa(fa_), size(size_), pre(pre_), nxt(nxt_),
                                                                        is_leaf(ifleaf) {
                for (int i = 0; i < MAXNUM + 3; ++i) {
                    address[i] = -1;
                }
            }

            node(const node &other) {
                fa = other.fa;
                size = other.size;
                is_leaf = other.is_leaf, pre = other.pre, nxt = other.nxt;
                for (int i = 0; i <= size; ++i) {
                    info[i] = other.info[i], address[i] = other.address[i];
                }

            }

            node &operator=(const node &other) {
                if (this == &other) return *this;
                fa = other.fa;
                size = other.size;
                is_leaf = other.is_leaf, pre = other.pre, nxt = other.nxt;
                for (int i = 0; i <= size; ++i) {
                    info[i] = other.info[i], address[i] = other.address[i];
                }
                return *this;
            }

            bool operator!=(const node &other) {
                if (this == &other) return false;
                if (fa != other.fa) return true;
                if (size != other.size) return true;
                if (is_leaf != other.is_leaf) return true;
                if (pre != other.pre) return true;
                if (nxt != other.nxt) return true;
                for (int i = 0; i <= size; ++i) {
                    if (info[i] != other.info[i]) return true;
                }
                for (int i = 0; i <= size; ++i) {
                    if (address[i] != other.address[i]) return true;
                }
                return false;
            }

        };

    public:
        class treebasic_info {
        public:
            int root = -1;
            int head_leaf = -1;
            int current_size = 0;
            int treefile_end = -1;
        };

    public:
        bool empty() {
            return basic.root == -1;
        }

        int searchforleaf(value_type temp, node &pos) {//node 设置为引用传递，让node 进入该函数时能够一直改变 返回时候 pos变成叶子节点
            Key tempkey = temp.first;
            T tempvalue = temp.second;
            int nxtnode = basic.root;

            if (nxtnode == -1) return -1;

            f11.read(nxtnode, pos);
//            f1.seekg(nxtnode);
//            f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
            // showleaf(pos);

            while (pos.nxt == 0) {
                int i;
                for (i = 1; i <= pos.size; ++i) {
                    if (tempkey < pos.info[i]) break;
                }
                if (tempkey > pos.info[pos.size]) nxtnode = pos.address[pos.size];
                else nxtnode = pos.address[i - 1];
                //特判一下
                f11.read(nxtnode, pos);
//                f1.seekg(nxtnode);
//                f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
                // showleaf(pos);
            }
            return nxtnode;
        }

        std::vector<value_type> findall() {
            std::vector<value_type> ans;
            ans.clear();
            if (empty()) {
                return ans;
            }
            node pos;
            int nxtnode = basic.root;
            f11.read(nxtnode, pos);
//            f1.seekg(nxtnode);
//            f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
            //   showleaf(pos);
            if (pos.nxt == -1) {
                for (int i = 1; i <= pos.size; ++i) {
                    value_type read;

                    f22.file.close();
                    f22.file.open(dataname, std::ios_base::binary | std::ios::in | std::ios::out);
                    if (!f22.file) { f22.file.open(dataname, std::ios::out | std::ios::binary); }

                    f22.read(pos.address[i], read);

                    ans.push_back(read);
                }

            } else {
                while (pos.nxt == 0) {
                    //特判一下
                    f11.read(pos.address[0], pos);
//                    f1.seekg(pos.address[0]);
//                    f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
                    // showleaf(pos);
                }
                while (true) {
                    for (int i = 1; i <= pos.size; ++i) {
                        value_type read;
                        f22.file.close();
                        f22.file.open(dataname, std::ios_base::binary | std::ios::in | std::ios::out);
                        if (!f22.file) { f22.file.open(dataname, std::ios::out | std::ios::binary); }
                        f22.read(pos.address[i], read);
                        ans.push_back(read);
                    }
                    if (pos.nxt == -1) break;
                    f11.read(pos.nxt, pos);
//                    f1.seekg(pos.nxt);
//                    f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
                }

            }

//todo
            return ans;


        }

        bool update(value_type pre) {
            Key tempkey = pre.first;
            T tempdata = pre.second;
            node ntr;
            int erasepos = searchforleaf(pre, ntr);
            if (erasepos == -1) return false;
            while (true) {
                for (int i = 1; i <= ntr.size; ++i) {
                    if (ntr.info[i] > tempkey) break;
                    if (ntr.info[i] < tempkey) continue;
                    if (ntr.info[i] == tempkey) {
                        value_type nowvaluetype=pre;
                        f22.write(ntr.address[i], nowvaluetype);
                        return true;
                    }
                }
                erasepos = ntr.pre;
                if (erasepos == -1) break;
                f11.read(erasepos, ntr);
                if (ntr.info[ntr.size] < tempkey) break;
            }
            return false;
        }

        std::vector<value_type> rangefind(Key key1, Key key2) {

        }

        bool exist(Key tempkey) {
            std::vector<value_type> ans = find(tempkey);
            if (ans.size() == 0) return false;
            else return true;
        }

        std::vector<value_type> find(Key tempkey) {
            std::vector<value_type> ans;
            ans.clear();
            if (empty()) {
                return ans;
            }
            node tempnode;
            T a;
            value_type tempval(tempkey, a);
            int pos = searchforleaf(tempval, tempnode);
            while (true) {
                node leafnode;
                f11.read(pos, leafnode);
                // f1.seekg(pos);
                int i;
                //f1.read(reinterpret_cast<char *>(&leafnode), sizeof(node));
                for (i = 1; i <= leafnode.size; ++i) {
                    if (tempkey < leafnode.info[i])break;
                    else if (tempkey > leafnode.info[i]) continue;
                    else {
                        value_type read;
                        f22.file.close();
                        f22.file.open(dataname, std::ios_base::binary | std::ios::in | std::ios::out);
                        if (!f22.file) { f22.file.open(dataname, std::ios::out | std::ios::binary); }
                        f22.read(leafnode.address[i], read);

                        ans.push_back(read);
                    }
                }
                int pre = leafnode.pre;
                if (pre == -1) break;
                pos = pre;
                f11.read(pre, leafnode);
//                f1.seekg(pre);
//                f1.read(reinterpret_cast<char *>(&leafnode), sizeof(node));
                if (leafnode.info[leafnode.size] < tempkey) break;
            }
            return ans;
        }

        void insert(value_type temp) {
            Key tempkey = temp.first;
            T tempvalue = temp.second;
            f11.file.close();
            f11.file.open(treename, std::ios_base::binary | std::ios::in | std::ios::out);
            if (!f11.file) { f11.file.open(treename, std::ios::out | std::ios::binary); }
            f11.file.seekg(0, std::ios::end);
            f22.file.seekg(0, std::ios::end);
            int treeend = f11.file.tellg(), dataend = f22.file.tellg();
            if (empty()) {
                node node1(-1, 1, -1, -1, 1);
                node1.info[1] = tempkey;
                node1.address[1] = dataend;

                basic.root = treeend;
                f11.write(treeend, node1);


                f22.file.clear();
                f22.file.close();
                f22.file.open(dataname, std::ios_base::binary | std::ios::in | std::ios::out);
                if (!f22.file) { f22.file.open(dataname, std::ios::out | std::ios::binary); }
                f22.write(dataend, temp);
                basic.current_size++;


                f11.file.close();
                f11.file.open(treename, std::ios_base::binary | std::ios::in | std::ios::out);
                if (!f11.file) { f11.file.open(treename, std::ios::out | std::ios::binary); }

            } else {

                node tempnode;
                int nodepos = searchforleaf(temp, tempnode);//node已经调整为叶子节点，nodepos为该叶子节点的起始地址
                int i;

                if (!(tempkey < tempnode.info[tempnode.size])) {
                    i = tempnode.size + 1;
                } else {
                    for (i = 1; i <= tempnode.size; ++i) {
                        if (tempkey < tempnode.info[i])break;
                    }
                }//边界情况，要插入的数比最大的数还要大
                for (int j = tempnode.size; j >= i; --j) {
                    tempnode.info[j + 1] = tempnode.info[j];
                    tempnode.address[j + 1] = tempnode.address[j];
                }
                tempnode.size++;
                tempnode.info[i] = tempkey;
                tempnode.address[i] = dataend;

                f11.write(nodepos, tempnode);
                f22.write(dataend, temp);

                checkandsplit(nodepos, tempnode);
            }

        }

        void checkandsplit(int address, node temp) {
            int nowadress = address;
            int broadress;
            node ntr(temp);
            while (ntr.size == MAXNUM) {
                Key k = ntr.info[MAXNUM / 2 + 1];
                node bronode(ntr.fa, 0, nowadress, ntr.nxt, 1);
                if (ntr.nxt != 0) {
                    int size1 = MAXNUM / 2;
                    ntr.size = size1;
                    int size2 = MAXNUM - size1;
                    bronode.size = size2;


                    for (int i = 1; i <= size2; ++i) {
                        bronode.address[i] = ntr.address[size1 + i];
                        bronode.info[i] = ntr.info[size1 + i];
                    }
                    bronode.pre = nowadress;
                    f11.file.seekg(0, std::ios::end);
                    broadress = f11.file.tellg();
                    if (ntr.nxt != -1) {

                        node nxttemp;
                        f11.read(ntr.nxt, nxttemp);

                        nxttemp.pre = broadress;
                        f11.write(ntr.nxt, nxttemp);

                    }

                    ntr.nxt = broadress;
                    f11.write(nowadress, ntr);
                    f11.write(broadress, bronode);

                } else {
                    int size2 = MAXNUM - MAXNUM / 2 - 1;
                    bronode.size = size2;
                    bronode.is_leaf = 0;
                    ntr.size = MAXNUM / 2;
                    int size1 = MAXNUM / 2;
                    bronode.size = MAXNUM - ntr.size - 1;
                    for (int i = 0; i <= bronode.size; ++i) {
                        bronode.address[i] = ntr.address[size1 + i + 1];
                        bronode.info[i] = ntr.info[size1 + i + 1];
                    }


                    f11.file.seekg(0, std::ios::end);
                    broadress = f11.file.tellg();

                    f11.write(broadress, bronode);

                    ntr.nxt = 0;
                    f11.write(nowadress, ntr);


                    for (int i = 0; i <= bronode.size; ++i) {//s
                        // f1.seekg(bronode.address[i]);
                        node sonnode;
                        f11.read(bronode.address[i], sonnode);
                        //f1.read(reinterpret_cast<char *>(&sonnode), sizeof(node));
                        sonnode.fa = broadress;
                        f11.write(bronode.address[i], sonnode);
                    }


                }


                if (ntr.fa == -1) {
                    //构造新的根节点
                    node root(-1, 1, -1, 0, 0);
                    root.address[0] = nowadress;
                    root.address[1] = broadress;
                    root.info[1] = k;
                    f11.file.seekg(0, std::ios::end);
                    int rootpos = f11.file.tellg();
                    ntr.fa = rootpos;
                    bronode.fa = rootpos;
                    basic.root = rootpos;
                    f11.write(rootpos, root);
                    f11.write(nowadress, ntr);
                    f11.write(broadress, bronode);

                    break;
                } else {


                    int fa_address = ntr.fa;
                    f11.file.close();
                    f11.file.open(treename, std::ios_base::binary | std::ios::in | std::ios::out);
                    if (!f11.file) { f11.file.open(treename, std::ios::out | std::ios::binary); }
                    f11.read(fa_address, ntr);


                    int i;
                    for (i = 0; i <= ntr.size; ++i) {
                        if (ntr.address[i] == nowadress) break;
                    }
                    for (int j = ntr.size + 1; j > i + 1; --j) {
                        ntr.address[j] = ntr.address[j - 1];
                        ntr.info[j] = ntr.info[j - 1];
                    }
                    ntr.info[i + 1] = k;
                    ntr.address[i + 1] = broadress;
                    ntr.size++;
                    nowadress = fa_address;
                    f11.write(fa_address, ntr);
                }
            }
        }


        //todo
    public:
        bool erase(value_type tempval) {


            Key tempkey = tempval.first;
            T tempdata = tempval.second;
            node ntr;
            int erasepos = searchforleaf(tempval, ntr);
            if (erasepos == -1) return false;
            while (true) {
                for (int i = 1; i <= ntr.size; ++i) {
                    if (ntr.info[i] > tempkey) break;
                    if (ntr.info[i] < tempkey) continue;
                    if (ntr.info[i] == tempkey) {
                        value_type nowvaluetype;
                        f22.read(ntr.address[i], nowvaluetype);
                        if (nowvaluetype.second == tempdata) {
                            for (int j = i; j < ntr.size; ++j) {
                                ntr.info[j] = ntr.info[j + 1];
                                ntr.address[j] = ntr.address[j + 1];
                            }
                            ntr.size--;
                            f11.write(erasepos, ntr);
                            if (i == 1) {
                                changehead(ntr, erasepos, ntr.info[1]);
                            }
                            if (erasepos != basic.root) {
                                eraseadjust(erasepos, ntr);
                            } else {
                                //开始小数据的情况

                                if (ntr.size == 0) {
                                    basic.root = -1;
                                }
                            }
                            return true;
                        }
                    }

                }
                erasepos = ntr.pre;
                if (erasepos == -1) break;
                f11.read(erasepos, ntr);

                if (ntr.info[ntr.size] < tempkey) break;
            }
            return false;
        }

        void changehead(node ntr, int erasepos, Key tempdata) {
            node tempnode(ntr), fa;
            int i;
            int ntrpos = erasepos, fa_pos = ntr.fa;
            while (fa_pos != -1) {
                f11.read(fa_pos, fa);
                for (i = 0; i <= fa.size; ++i) {
                    if (ntrpos == fa.address[i]) break;
                }
                if (i == 0) {
                    ntrpos = fa_pos;
                    fa_pos = fa.fa;

                } else {
                    fa.info[i] = tempdata;
                    f11.write(fa_pos, fa);
                    break;
                }
            }
        }


        //todo still have problems
        void eraseadjust(int pos, node erasenode) {
            node nownode(erasenode), rbro, lbro, fa;
            int nowpos = pos, fa_pos;
            while (true) {
                if (nownode.fa == -1) break;
                if (nownode.nxt != 0) {
                    //merge leave node
                    if (nownode.size >= MINNUM) break;
                    if (nownode.fa == -1) break;//开始时候的情况
                    if (nownode.nxt != -1) {
                        f11.read(nownode.nxt, rbro);
                        if (rbro.fa == nownode.fa && rbro.size >= MINNUM + 1) {
                            int n = MINNUM - nownode.size;
                            for (int i = 1; i <= n; ++i) {
                                nownode.address[i + nownode.size] = rbro.address[i];
                                nownode.info[i + nownode.size] = rbro.info[i];
                            }
                            for (int i = 1; i <= rbro.size - n; ++i) {
                                rbro.address[i] = rbro.address[i + n];
                                rbro.info[i] = rbro.info[i + n];
                            }
                            rbro.size -= n;
                            nownode.size += n;
                            f11.write(nowpos, nownode);
                            f11.write(nownode.nxt, rbro);

                            //adjust the fa
                            f11.read(nownode.fa, fa);
                            int i;
                            for (i = 0; i <= fa.size; ++i) {
                                if (fa.address[i] == nowpos) break;
                            }
                            fa.info[i + 1] = rbro.info[1];
                            f11.write(nownode.fa, fa);
                            break;
                        }
                    }//borrow from right
                    if (nownode.pre != -1) {
                        f11.read(nownode.pre, lbro);
                        if (lbro.fa == nownode.fa && lbro.size >= MINNUM + 1) {
                            int n = MINNUM - nownode.size;


                            //todo
                            for (int i = nownode.size + n; i > n; --i) {
                                nownode.info[i] = nownode.info[i - n];
                                nownode.address[i] = nownode.address[i - n];
                            }
                            for (int i = 1; i <= n; ++i) {
                                nownode.info[i] = lbro.info[lbro.size - n + i];
                                nownode.address[i] = lbro.address[lbro.size - n + i];
                            }


//                            for (int i = 1; i <= n; ++i) {
//                                nownode.address[i + nownode.size] = lbro.address[i];
//                                nownode.info[i + nownode.size] = lbro.info[i];
//                            }
//                            for (int i = 1; i <= lbro.size - n; ++i) {
//                                lbro.address[i] = lbro.address[i + n];
//                                lbro.info[i] = lbro.info[i + n];
//                            }
                            lbro.size -= n;
                            nownode.size += n;
                            f11.write(nowpos, nownode);
                            f11.write(nownode.pre, lbro);
                            //adjust the fa
                            f11.read(nownode.fa, fa);
                            int i;
                            for (i = 0; i <= fa.size; ++i) {
                                if (fa.address[i] == nowpos) break;
                            }
                            //bug in 5/29 10:59
                            fa.info[i] = nownode.info[1];
                            f11.write(nownode.fa, fa);
                            break;
                        }
                    }//borrow from left


                    //merge_right
                    if (nownode.nxt != -1 && rbro.fa == nownode.fa) {
                        for (int i = 1; i <= rbro.size; ++i) {
                            nownode.info[i + nownode.size] = rbro.info[i];
                            nownode.address[i + nownode.size] = rbro.address[i];
                        }
                        nownode.size += rbro.size;
                        nownode.nxt = rbro.nxt;
                        if (rbro.nxt != -1) {

                            node temp;
                            f11.read(rbro.nxt, temp);
                            temp.pre = nowpos;
                            f11.write(rbro.nxt, temp);

                        }
                        f11.write(nowpos, nownode);

                        //modify the current floor


                        //to modify the fa
                        f11.read(nownode.fa, fa);

                        int control;
                        for (control = 0; control <= fa.size; ++control) {
                            if (fa.address[control] == nowpos) break;
                        }
                        for (int i = control + 1; i < fa.size; ++i) {
                            fa.address[i] = fa.address[i + 1];
                            fa.info[i] = fa.info[i + 1];
                        }
                        fa.size--;
                        f11.write(nownode.fa, fa);

                        if (fa.size == 0) {
                            basic.root = nowpos;

                            node roottemp;
                            f11.read(nowpos, roottemp);

                            roottemp.fa = -1;
                            f11.write(nowpos, roottemp);
                            break;
                        }
                        nowpos = nownode.fa;

                        nownode = fa;
                        f11.write(nowpos, nownode);
                        continue;
                    }

                    //merge_left
                    //todo
                    if (nownode.pre != -1 && lbro.fa == nownode.fa) {
                        for (int i = lbro.size + 1; i <= nownode.size + lbro.size; ++i) {
                            nownode.address[i] = nownode.address[i - lbro.size];
                            nownode.info[i] = nownode.info[i - lbro.size];
                        }
                        for (int i = 1; i <= lbro.size; ++i) {
                            nownode.address[i] = lbro.address[i];
                            nownode.info[i] = lbro.info[i];
                        }
                        nownode.pre = lbro.pre;
                        nownode.size += lbro.size;
                        if (lbro.pre != -1) {

                            node templl;
                            f11.read(lbro.pre, templl);

                            templl.nxt = nowpos;
                            f11.write(lbro.pre, templl);
                        } else {
                            basic.head_leaf = nowpos;
                        }
                        f11.write(nowpos, nownode);




                        // change the fa_info

                        int fatemppos = nownode.fa;
                        f11.read(fatemppos, fa);

                        int i;
                        for (i = 0; i <= fa.size; ++i) {
                            if (fa.address[i] == nowpos) break;
                        }
                        fa.info[i] = fa.info[i - 1];//精髓
                        for (int j = i - 1; j <= fa.size - 1; ++j) {
                            fa.address[j] = fa.address[j + 1];
                            fa.info[j] = fa.info[j + 1];
                        }
                        fa.size--;
                        f11.write(fatemppos, fa);

                        if (fa.size == 0) {
                            basic.root = nowpos;

                            node roottemp;
                            f11.read(nowpos, roottemp);

                            roottemp.fa = -1;
                            f11.write(nowpos, roottemp);

                            break;
                        }
                        nowpos = nownode.fa;

                        nownode = fa;
                        f11.write(nowpos, nownode);

                        continue;
                    }


                } else
                    // to cope with the condition of internal leaf
                {
                    if (nownode.size >= MINNUM) break;//todo may have problem  ???

                    int fapos = nownode.fa;
                    f11.read(fapos, fa);
                    int i;
                    for (i = 0; i <= fa.size; ++i) {
                        if (fa.address[i] == nowpos) break;
                    }
                    node internal_right;
                    node internal_left;
                    //borrow from right
                    if (i < fa.size) {
                        //borrow from the right
                        f11.read(fa.address[i + 1], internal_right);
                        if (internal_right.size >= MINNUM + 1) {
                            int borrow_pos = internal_right.address[0];

                            node trmpborrow;
                            f11.read(borrow_pos, trmpborrow);

                            trmpborrow.fa = nowpos;
                            f11.write(borrow_pos, trmpborrow);
                            Key borrowkey = fa.info[i + 1];
                            fa.info[i + 1] = internal_right.info[1];
                            f11.write(fapos, fa);
                            for (int j = 0; j <= internal_right.size - 1; ++j) {
                                internal_right.address[j] = internal_right.address[j + 1];
                            }
                            for (int j = 1; j <= internal_right.size - 1; ++j) {
                                internal_right.info[j] = internal_right.info[j + 1];
                            }
                            internal_right.size--;
                            f11.write(fa.address[i + 1], internal_right);

//忘记修改父亲节点的info值！！！大bug

                            nownode.info[nownode.size + 1] = borrowkey;//todo
                            nownode.address[nownode.size + 1] = borrow_pos;
                            nownode.size++;
                            f11.write(nowpos, nownode);


                            break;
                        }


                    }
                    //borrow from left
                    if (i > 0) {
                        f11.read(fa.address[i - 1], internal_left);
                        if (internal_left.size >= MINNUM + 1) {
                            int borrow_pos = internal_left.address[internal_left.size];

                            node trmpborrow;
                            f11.read(borrow_pos, trmpborrow);

                            trmpborrow.fa = nowpos;
                            f11.write(borrow_pos, trmpborrow);


                            Key borrowkey = fa.info[i];
                            fa.info[i] = internal_left.info[internal_left.size];
                            f11.write(fapos, fa);
                            internal_left.size--;
                            f11.write(fa.address[i - 1], internal_left);
                            for (int j = nownode.size + 1; j >= 1; --j) {
                                nownode.address[j] = nownode.address[j - 1];
                                nownode.info[j] = nownode.info[j - 1];
                            }
                            nownode.address[1] = nownode.address[0];
                            nownode.address[0] = borrow_pos;
                            nownode.info[1] = borrowkey;
                            nownode.size++;
                            f11.write(nowpos, nownode);
                            break;
                        }
                    }





//todo
                    //merge from right
                    if (i < fa.size) {
                        nownode.size++;
                        //todo wrong!!!
                        for (int j = 0; j <= internal_right.size; ++j) {
                            nownode.address[nownode.size + j] = internal_right.address[j];
                            nownode.info[nownode.size + j] = internal_right.info[j];//wrong
                            node tempsontochange;
                            f11.read(internal_right.address[j], tempsontochange);
                            tempsontochange.fa = nowpos;
                            f11.write(internal_right.address[j], tempsontochange);
                        }
                        nownode.info[nownode.size] = fa.info[i + 1];
                        nownode.size += internal_right.size;
                        fa_pos = nownode.fa;
                        f11.write(nowpos, nownode);
                        nownode = fa;
                        for (int j = i + 1; j < nownode.size; ++j) {
                            nownode.info[j] = nownode.info[j + 1];
                            nownode.address[j] = nownode.address[j + 1];
                        }
                        nownode.size--;
                        if (nownode.size == 0) {
                            basic.root = nowpos;

                            node roottemp;
                            f11.read(nowpos, roottemp);

                            roottemp.fa = -1;
                            f11.write(nowpos, roottemp);
                            break;
                        }
                        nowpos = fa_pos;
                        f11.write(nowpos, nownode);
                        continue;
                    }
                    //merge from left
                    if (i > 0) {
                        nownode.info[0] = fa.info[i];
                        for (int j = 0; j <= nownode.size; ++j) {
                            nownode.info[internal_left.size + 1 + j] = nownode.info[j];
                            nownode.address[internal_left.size + 1 + j] = nownode.address[j];
                        }
                        fa_pos = nownode.fa;
                        for (int j = 0; j <= internal_left.size; ++j) {
                            nownode.info[j] = internal_left.info[j];
                            nownode.address[j] = internal_left.address[j];
                            node tempnode;
                            f11.read(internal_left.address[j], tempnode);

                            tempnode.fa = nowpos;
                            f11.write(internal_left.address[j], tempnode);
                        }
                        nownode.address[0] = internal_left.address[0];

                        node tempnode;
                        f11.read(internal_left.address[0], tempnode);

                        tempnode.fa = nowpos;
                        f11.write(internal_left.address[0], tempnode);
                        nownode.size += (1 + internal_left.size);
                        f11.write(nowpos, nownode);

                        nownode = fa;
                        nownode.info[i] = nownode.info[i - 1];

                        //todo
                        for (int j = i - 1; j < nownode.size; ++j) {
                            nownode.info[j] = nownode.info[j + 1];
                            nownode.address[j] = nownode.address[j + 1];
                        }

                        nownode.size--;
                        if (nownode.size == 0) {//todo
                            basic.root = nowpos;

                            node roottemp;
                            f11.read(nowpos, roottemp);

                            roottemp.fa = -1;
                            f11.write(nowpos, roottemp);
                            break;
                        }
                        nowpos = fa_pos;
                        f11.write(nowpos, nownode);
                        continue;
                    }
                }
            }
        }

        T &operator[](const Key &key) {
            std::vector<value_type> tmp = find(key);
            if (tmp.size() == 0) {
                value_type insert1(key, T());
                insert(insert1);

                std::vector<value_type> tmp1 = find(key);

                cout << tmp1[0].second << "&&&&&inbpt  size==0" << endl;
                return tmp1[0].second;


            } else {
                cout << tmp[0].second << "&&&&&inbpt" << endl;
                return tmp[0].second;

            }
        }


        void clear() {
            f11.file.open(treename, std::ios::out | std::ios::binary);
            f11.file.close();
            f22.file.open(dataname, std::ios::out | std::ios::binary);
            f22.file.close();

        }

        void showleaf(node a) {
            std::cout << "_____________________________/debug node" << std::endl;
            std::cout << "size:    " << a.size << "     pre:" << a.pre << "    nxt:" << a.nxt << "  fa:    " << a.fa
                      << std::endl;
            for (int i = 0; i <= a.size; ++i) {
                std::cout << "  pos:" << i << "    address[]" << a.address[i] << "     info:" << a.info[i] << std::endl;
            }
            std::cout << "_____________________________/debug node" << std::endl;
            std::cout << std::endl;
        }

        friend ostream &operator<<(ostream &output,
                                   const node &a) {
            output << "_____________________________/debug node";
            output << "size:    " << a.size << "     pre:" << a.pre << "    nxt:" << a.nxt << "  fa:    " << a.fa
                   << "std::endl     ";
            for (int i = 0; i <= a.size; ++i) {
                output << "  pos:" << i << "    address[]" << a.address[i] << "     info:" << a.info[i] << "    ";
            }
            output << "_____________________________/debug node";


            return output;
        }

    public:
        Diskmanager<node, 500> f11;
        Diskmanager<value_type, 500> f22;
        // std::fstream f1, f2;//f1是索引文档，f2是数据文档
        int sum_size;
        int data_end;//may be useless
        treebasic_info basic;
        std::string treename, dataname;

        BPtree() {

        };

        BPtree(std::string a, std::string b) : f11(a), f22(b) {

            treename = a, dataname = b;
            f11.file.seekg(0, std::ios::end);
            int head_search = f11.file.tellg();
            if (head_search == 0) {
                treebasic_info temp;
                f11.file.close();
                f11.file.open(a, std::ios_base::binary | std::ios::in | std::ios::out);
                if (!f11.file) { f11.file.open(a, std::ios::out | std::ios::binary); }
                f11.file.seekg(0);
                f11.file.write(reinterpret_cast<char *>(&temp), sizeof(treebasic_info));
            } else {

                treebasic_info temp;
                f11.file.close();
                f11.file.open(a, std::ios_base::binary | std::ios::in | std::ios::out);
                if (!f11.file) { f11.file.open(a, std::ios::out | std::ios::binary); }
                f11.file.seekg(0);
                f11.file.read(reinterpret_cast<char *>(&temp), sizeof(treebasic_info));
                basic.root = temp.root;
                basic.treefile_end = temp.treefile_end;
                basic.current_size = temp.current_size;
                basic.head_leaf = temp.head_leaf;
            }
        }

        ~BPtree() {
            treebasic_info temp;
            temp.root = basic.root;
            temp.current_size = basic.current_size;
            temp.head_leaf = basic.head_leaf;
            f11.file.close();
            f11.file.open(treename, std::ios_base::binary | std::ios::in | std::ios::out);
            if (!f11.file) { f11.file.open(treename, std::ios::out | std::ios::binary); }
            f11.file.seekg(0);
            f11.file.write(reinterpret_cast<char *>(&temp), sizeof(treebasic_info));
            f11.file.close(), f22.file.close();


            f11.file.seekg(0, std::ios::end);
            int head_search = f11.file.tellg();

        }

    };
}
#endif //BPLUSTREE_BPLUSTREE_H

#include <iostream>

using namespace std;

#include "bplustree.hpp"
#include <vector>
#include <map>
#include <ctime>
const int MAXN=20;
int main() {
    srand(time(0));
    sjtu::BPtree<int, int> bptree("bpt", "data");
    map<int, int> stdmp;

    vector<sjtu::BPtree<int, int>::value_type> ans;
    vector<int > record;
    for (int i = 1; i <=200 ; i++) {


        int P = rand() % 1000 + 8;
        int Q = rand() % 10000 + 2;


        //cout << P << "  " << Q << endl;
        //  cout << "insert    :  before " << i << endl;
        sjtu::BPtree<int, int>::value_type a(i, i);
        stdmp.insert(a);
      bptree[i]=i;

      cout<<bptree[i]<<"&&&&"<<endl;
        cout << "insert    :   " << i << endl;
        //
        //

//        sjtu::BPtree<int, int>::treebasic_info inf;
//        bptree.f1.seekg(0);
//        bptree.f1.read(reinterpret_cast<char *>(&inf), sizeof(sjtu::BPtree<int, int>::treebasic_info));
//        cout << inf.root<<"    "<<i << endl;
//        cout<<inf.head_leaf<<"     "<<inf.current_size<<"   "<<inf.treefile_end<<endl;
//
//
//        sjtu::BPtree<int, int>::node tempnode;


//        std::map<int, int>::iterator it;
//        it = stdmp.find(i);
//        ans = bptree.find(i);
//        {
//            cout << i << "   ";
//            cout << it->second << "&&&&&&&&&&&&&&&&&&&&&" << endl;
//            cout << ans.size()<< "***********" << endl;
//        }


    }

    for (int i = 1; i <=200; ++i){
        cout<<i<<"****"<<endl;
        cout<<bptree[i]<<endl;

    }



//    for (int i = 1; i <= 50; ++i) {
//
//        int Q = rand()%10000+2;
//        int P=rand()%1000+8;
//        //todo
//        cout<<P<<"  "<<Q<<endl;
//       // cout << "insert    :  before " << i << endl;
//        sjtu::BPtree<int, int>::value_type a(P, Q);
//        stdmp.insert(a);
//        bptree.insert(a);
//       // cout << "insert    :   " << i << endl;
//
//
//        std::map<int, int>::iterator it;
//        it = stdmp.find(P);
//        ans = bptree.find(P);
//       // cout << ans.size() << "&&&&" << i << endl;
//       cout<<i<<"    num now"<<endl;
//        cout << it->second << "&&&&&&&&&&&&&&&&&&&&&" << endl;
//        cout << ans[0].second << "***********" << endl;
//    }


    //可以测试大数据点的情况
//    for (int i = 1; i <= 5000; ++i) {
//        std::map<int, int>::iterator it;
//        it = stdmp.find(i);
//        ans = bptree.find(i);
//        cout << ans.size() << "&&&&" << i << endl;
//        cout << it->second << "&&&" << endl;
//        cout << ans[0].second << "***" << endl;
//    }
    cout << "****" << endl;
    //std::cout << "Hello, World!" << std::endl;
    return 0;
}